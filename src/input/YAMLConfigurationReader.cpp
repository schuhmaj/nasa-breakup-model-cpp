#include "YAMLConfigurationReader.h"

double YAMLConfigurationReader::getMinimalCharacteristicLength() const {
    if (_file["minimalCharacteristicLength"]) {
        return _file["minimalCharacteristicLength"].as<double>();
    } else {
        return 0.05;
    }
}

SimulationType YAMLConfigurationReader::getTypeOfSimulation() const {
    if (_file["simulationType"]) {
        try {
            SimulationType simulationType = InputConfigurationSource::stringToSimulationType.at(
                    _file["simulationType"].as<std::string>());
            return simulationType;
        } catch (std::exception &e) {
            return SimulationType::UNKNOWN;
        }
    } else {
        return SimulationType::UNKNOWN;
    }
}

size_t YAMLConfigurationReader::getCurrentMaximalGivenID() const {
    if (_file["currentMaxID"]) {
        return _file["currentMaxID"].as<size_t>();
    } else {
        return 0;
    }
}

std::shared_ptr<DataSource> YAMLConfigurationReader::getDataReader() const {
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
        if (fileNames[0].find(".csv") && fileNames[1].find(".txt")){
            return std::make_shared<TLESatcatDataReader>(fileNames[0], fileNames[1]);
        //fileName.tle (should be tle) && fileName.csv (should be satcat)
        } else if (fileNames[0].find(".txt") && fileNames[1].find(".csv")) {
            return std::make_shared<TLESatcatDataReader>(fileNames[1], fileNames[0]);
        }
    }
    throw std::runtime_error{"Data file input is not correctly set-up!"};
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
            if (filename.substr(filename.size()-3) == "csv") {
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
            } else if (filename.substr(filename.size()-3) == "vtu") {
                outputs.push_back(std::shared_ptr<OutputWriter>(new VTKWriter(filename)));
            } else {
                throw std::runtime_error{"Invalid output target inside the YAML file!"};
            }
        }
    }
    return outputs;
}
