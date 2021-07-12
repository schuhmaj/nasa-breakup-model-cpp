#include "YAMLConfigurationReader.h"

/*
 * TODO: Refactor this stuff and move some checks into a centralized form, maybe constructor!
 */

double YAMLConfigurationReader::getMinimalCharacteristicLength() const {
    if (_file["simulation"] && _file["simulation"]["minimalCharacteristicLength"]) {
        return _file["simulation"]["minimalCharacteristicLength"].as<double>();
    } else {
        throw std::runtime_error{"The minimal characteristic Length was not specified in the YAML Configuration file!"};
    }
}

SimulationType YAMLConfigurationReader::getTypeOfSimulation() const {
    if (_file["simulation"] && _file["simulation"]["simulationType"]) {
        try {
            SimulationType simulationType = InputConfigurationSource::stringToSimulationType.at(
                    _file["simulation"]["simulationType"].as<std::string>());
            return simulationType;
        } catch (std::exception &e) {
            spdlog::warn("The simulation type could not be parsed from the YAML Configuration file! "
                         "SimulationType therefore UNKNOWN!");
            return SimulationType::UNKNOWN;
        }
    } else {
        spdlog::warn("The simulation was not given in the YAML Configuration file! "
                     "SimulationType therefore UNKNOWN!");
        return SimulationType::UNKNOWN;
    }
}

std::optional<size_t> YAMLConfigurationReader::getCurrentMaximalGivenID() const {
    if (_file["simulation"] && _file["simulation"]["currentMaxID"]) {
        return std::make_optional(_file["simulation"]["currentMaxID"].as<size_t>());
    } else {
        return std::nullopt;
    }
}

std::shared_ptr<const DataSource> YAMLConfigurationReader::getDataReader() const {
    std::vector<std::string> fileNames{};
    if (_file["simulation"] && _file["simulation"]["inputSource"] && _file["simulation"]["inputSource"].IsSequence()) {
        for (auto inputSource : _file["simulation"]["inputSource"]) {
            fileNames.push_back(inputSource.as<std::string>());
        }
    }

    //fileName.yaml
    if (fileNames.size() == 1 && fileNames[0].find(".yaml")) {
        return std::make_shared<YAMLDataReader>(fileNames[0]);
    } else if (fileNames.size() == 2) {
        //fileName.csv (should be satcat) && fileName.txt (should be tle)
        if (fileNames[0].find(".csv") && fileNames[1].find(".txt")) {
            return std::make_shared<TLESatcatDataReader>(fileNames[0], fileNames[1]);
            //fileName.tle (should be tle) && fileName.csv (should be satcat)
        } else if (fileNames[0].find(".txt") && fileNames[1].find(".csv")) {
            return std::make_shared<TLESatcatDataReader>(fileNames[1], fileNames[0]);
        }
    }
    //Error Handling
    std::stringstream message{};
    message << "The YAML Configuration Reader parsed the following files as Data Input:\n";
    for (unsigned int i = 0; i < fileNames.size(); ++i) {
        message << '{' << i << ": " << fileNames[i] << "}\n";
    }
    message << "This is no valid configuration!";
    throw std::runtime_error{message.str()};
}

std::optional<std::set<size_t>> YAMLConfigurationReader::getIDFilter() const {
    if (_file["simulation"] && _file["simulation"]["idFilter"] && _file["simulation"]["idFilter"].IsSequence()) {
        std::set<size_t> filterSet{};
        for (auto id : _file["simulation"]["idFilter"]) {
            filterSet.insert(id.as<size_t>());
        }
        return std::make_optional(filterSet);
    }
    return std::nullopt;
}

std::vector<std::shared_ptr<OutputWriter>> YAMLConfigurationReader::getOutputTargets() const {
    if (_file["resultOutput"] && _file["resultOutput"]["target"] && _file["resultOutput"]["target"].IsSequence()) {
        return extractOutputWriter(_file["resultOutput"]);
    } else {
        spdlog::info("You have defined no way of output for the result of the simulation!");
    }
    return std::vector<std::shared_ptr<OutputWriter>>{};
}

std::vector<std::shared_ptr<OutputWriter>> YAMLConfigurationReader::getInputTargets() const {
    std::vector<std::shared_ptr<OutputWriter>> outputs{};
    if (_file["inputOutput"] && _file["inputOutput"]["target"] && _file["inputOutput"]["target"].IsSequence()) {
        return extractOutputWriter(_file["inputOutput"]);
    } //This param is optional, so no info, if no output for input is specified
    return std::vector<std::shared_ptr<OutputWriter>>{};
}

std::vector<std::shared_ptr<OutputWriter>>
YAMLConfigurationReader::extractOutputWriter(const YAML::Node &node) {
    std::vector<std::shared_ptr<OutputWriter>> outputs{};
    for (auto outputFile : node["target"]) {
        std::string filename{outputFile.as<std::string>()};
        if (filename.substr(filename.size() - 3) == "csv") {            //CSV Case
            if (node["csvPattern"]) {
                auto pattern = node["csvPattern"].as<std::string>();
                outputs.push_back(std::shared_ptr<OutputWriter>(new CSVPatternWriter(filename, pattern)));
            } else {
                bool kepler = false;
                if (node["kepler"]) {
                    kepler = node["kepler"].as<bool>();
                }
                outputs.push_back(std::shared_ptr<OutputWriter>(new CSVWriter(filename, kepler)));
            }
        } else if (filename.substr(filename.size() - 3) == "vtu") {     //VTK Case
            outputs.push_back(std::shared_ptr<OutputWriter>(new VTKWriter(filename)));
        } else {
            spdlog::warn("The file {} is no available output form. Available are csv and vtu Output", filename);
        }
    }
    if (outputs.empty()) {
        spdlog::warn("You have defined OutputTarget/ InputTargets with no valid file formats!");
    }
    return outputs;
}
