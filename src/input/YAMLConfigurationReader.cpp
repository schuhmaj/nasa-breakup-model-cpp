#include "YAMLConfigurationReader.h"

double YAMLConfigurationReader::getMinimalCharacteristicLength() const {
    if (_file["minimalCharacteristicLength"]) {
        return _file["minimalCharacteristicLength"].as<double>();
    } else {
        throw std::runtime_error{"The minimal characteristic Length was not specified in the YAML Configuration file!"};
    }
}

SimulationType YAMLConfigurationReader::getTypeOfSimulation() const {
    if (_file["simulationType"]) {
        try {
            SimulationType simulationType = InputConfigurationSource::stringToSimulationType.at(
                    _file["simulationType"].as<std::string>());
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
    if (_file["currentMaxID"]) {
        return std::make_optional(_file["currentMaxID"].as<size_t>());
    } else {
        return std::nullopt;
    }
}

std::shared_ptr<const DataSource> YAMLConfigurationReader::getDataReader() const {
    std::vector<std::string> fileNames{};
    if (_file["inputSource"] && _file["inputSource"].IsSequence()) {
        for (auto inputSource : _file["inputSource"]) {
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
    if (_file["idFilter"] && _file["idFilter"].IsSequence()) {
        std::set<size_t> filterSet{};
        for (auto id : _file["idFilter"]) {
            filterSet.insert(id.as<size_t>());
        }
        return std::make_optional(filterSet);
    }
    return std::nullopt;
}

std::vector<std::shared_ptr<OutputWriter>> YAMLConfigurationReader::getOutputTargets() const {
    std::vector<std::shared_ptr<OutputWriter>> outputs{};
    if (_file["outputTarget"] && _file["outputTarget"].IsSequence()) {
        for (auto outputFile : _file["outputTarget"]) {
            std::string filename{outputFile.as<std::string>()};
            if (filename.substr(filename.size() - 3) == "csv") {
                if (_file["outputCSVPattern"]) {
                    auto pattern = _file["outputCSVPattern"].as<std::string>();
                    outputs.push_back(std::shared_ptr<OutputWriter>(new CSVPatternWriter(filename, pattern)));
                } else {
                    bool kepler = false;
                    if (_file["outputKepler"]) {
                        kepler = _file["outputKepler"].as<bool>();
                    }
                    outputs.push_back(std::shared_ptr<OutputWriter>(new CSVWriter(filename, kepler)));
                }
            } else if (filename.substr(filename.size() - 3) == "vtu") {
                outputs.push_back(std::shared_ptr<OutputWriter>(new VTKWriter(filename)));
            } else {
                spdlog::warn("The file {} is no available output form. Available are csv and vtu Output", filename);
            }
        }
        if (outputs.empty()) {
            spdlog::warn("You have defined OutputTarget with no valid files!");
        }
    } else {
        spdlog::info("You have defined no way of output!");
    }
    return outputs;
}
