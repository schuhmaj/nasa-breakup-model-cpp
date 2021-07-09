#include "YAMLDataReader.h"

std::vector<Satellite> YAMLDataReader::getSatelliteCollection() const {
    std::vector<Satellite> satelliteVector{};
    SatelliteBuilder satelliteBuilder{};

    if (_file["satellites"] && _file["satellites"].IsSequence()) {
        YAML::Node satellites{_file["satellites"]};
        for (auto satNode : satellites) {
            satelliteVector.push_back(parseSatellite(satelliteBuilder, satNode));
        }
    }
    return satelliteVector;
}

Satellite YAMLDataReader::parseSatellite(SatelliteBuilder &satelliteBuilder, const YAML::Node &node) {
    satelliteBuilder.reset();
    if (node["id"]) {
        satelliteBuilder.setID(node["id"].as<unsigned long>());
    }
    if (node["name"]) {
        satelliteBuilder.setName(node["name"].as<std::string>());
    }
    if (node["satType"]) {
        satelliteBuilder.setSatType(node["satType"].as<std::string>());
    }
    if (node["mass"]) {
        satelliteBuilder.setMass(node["mass"].as<double>());
    }
    if (node["area"]) {
        satelliteBuilder.setMassByArea(node["area"].as<double>());
    }
    if (node["velocity"]) {
        satelliteBuilder.setVelocity(node["velocity"].as<std::array<double, 3>>());
    }
    if (node["position"]) {
        satelliteBuilder.setPosition(node["position"].as<std::array<double, 3>>());
    }
    if (node["kepler"]) {
        parseKepler(satelliteBuilder, node["kepler"]);
    }
    return satelliteBuilder.getResult();
}

void YAMLDataReader::parseKepler(SatelliteBuilder &satelliteBuilder, const YAML::Node &node) {
    if (node["semi-major-axis"] && node["eccentricity"] && node["inclination"]
        && node["longitude-of-the-ascending-node"] && node["argument-of-periapsis"]) {
        OrbitalElementsFactory factory{};
        std::array<double, 6> keplerData{};
        keplerData[0] = node["semi-major-axis"].as<double>();
        keplerData[1] = node["eccentricity"].as<double>();
        keplerData[2] = node["inclination"].as<double>();
        keplerData[3] = node["longitude-of-the-ascending-node"].as<double>();
        keplerData[4] = node["argument-of-periapsis"].as<double>();

        if (node["eccentric-anomaly"]) {
            keplerData[5] = node["eccentric-anomaly"].as<double>();
            satelliteBuilder.setOrbitalElements(factory.fromOnlyRadians(keplerData, OrbitalAnomalyType::ECCENTRIC));
        } else if (node["mean-anomaly"]) {
            keplerData[5] = node["mean-anomaly"].as<double>();
            satelliteBuilder.setOrbitalElements(factory.fromOnlyRadians(keplerData, OrbitalAnomalyType::MEAN));
        } else if (node["true-anomaly"]) {
            keplerData[5] = node["true-anomaly"].as<double>();
            satelliteBuilder.setOrbitalElements(factory.fromOnlyRadians(keplerData, OrbitalAnomalyType::TRUE));
        } else {
            throw std::runtime_error{"One satellite input is incomplete!"
                                     "You have to give at least one of the following orbital Anomalies"
                                     "Eccentric Anomaly > Mean Anomaly > True Anomaly [in the order how the"
                                     "program will prioritize an anomaly if multiple are given]"};
        }
    } else {
        throw std::runtime_error{"One satellite input is incomplete!"
                                 "The Keplerian Elements are not fully given!"};
    }
}
