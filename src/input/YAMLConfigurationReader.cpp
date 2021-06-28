#include "YAMLConfigurationReader.h"

double YAMLConfigurationReader::getMinimalCharacteristicLength() {
    if (_file["minimalCharacteristicLength"]) {
        return _file["minimalCharacteristicLength"].as<double>();
    } else {
        return 0.05;
    }
}

SimulationType YAMLConfigurationReader::getTypeOfSimulation() {
    if (_file["simulationType"]) {
        try {
            SimulationType simulationType = ConfigurationSource::stringToSimulationType.at(
                    _file["simulationType"].as<std::string>());
            return simulationType;
        } catch (std::exception &e) {
            return SimulationType::UNKNOWN;
        }
    } else {
        return SimulationType::UNKNOWN;
    }
}

size_t YAMLConfigurationReader::getCurrentMaximalGivenID() {
    if (_file["currentMaxID"]) {
        return _file["currentMaxID"].as<size_t>();
    } else {
        return 0;
    }
}

std::shared_ptr<DataSource> YAMLConfigurationReader::getDataReader() {
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
            //TODO
        //fileName.tle (should be tle) && fileName.csv (should be satcat)
        } else if (fileNames[0].find(".txt") && fileNames[1].find(".csv")) {
            //TODO
        }
    } else {
        throw std::invalid_argument{"Data file input is not correctly set-up!"};
    }
}
