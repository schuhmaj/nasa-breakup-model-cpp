#include "YAMLDataReader.h"

std::vector<Satellite> YAMLDataReader::getSatelliteCollection() {
    std::vector<Satellite> satelliteVector{};
    SatelliteBuilder satelliteBuilder{};

    if (_file["satellites"] && _file["satellites"].IsSequence()) {
        YAML::Node satellites{_file["satellites"]};
        for (auto satNode : satellites) {
            try {
                satelliteVector.push_back(parseSatellite(satelliteBuilder, satNode));
            } catch (const std::exception& e) {
                std::cerr << e.what();
            }
        }
    }
    return satelliteVector;
}

Satellite YAMLDataReader::parseSatellite(SatelliteBuilder &satelliteBuilder, const YAML::Node& node) {
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
        std::array<double, 6> keplerianElements{};
        keplerianElements[0] = node["semi-major-axis"].as<double>();
        keplerianElements[1] = node["eccentricity"].as<double>();
        keplerianElements[2] = node["inclination"].as<double>();
        keplerianElements[3] = node["longitude-of-the-ascending-node"].as<double>();
        keplerianElements[4] = node["argument-of-periapsis"].as<double>();

        if (node["eccentric-anomaly"]) {
            keplerianElements[5] = node["eccentric-anomaly"].as<double>();
            satelliteBuilder.setKeplerianElementsEA(keplerianElements);
        } else if (node["mean-anomaly"]) {
            keplerianElements[5] = node["mean-anomaly"].as<double>();
            satelliteBuilder.setKeplerianElementsMA(keplerianElements);
        } else if (node["true-anomaly"]) {
            keplerianElements[5] = node["true-anomaly"].as<double>();
            satelliteBuilder.setKeplerianElementsTA(keplerianElements);
        } else {
            throw std::runtime_error{"You have to give at least one of the following orbital Anomalies"
                                        "Eccentric Anomaly > Mean Anomaly > True Anomaly [in the order how the"
                                        "program will prioritize an anomaly if multiple are given]"};
        }
    } else {
        throw std::runtime_error{"The Keplerian Elements are not fully given!"};
    }
}
