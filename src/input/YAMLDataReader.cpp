#include "YAMLDataReader.h"

std::vector<Satellite> YAMLDataReader::getSatelliteCollection() {
    std::vector<Satellite> satelliteVector{};
    SatelliteBuilder satelliteBuilder{};
    YAML::Node file{};

    try {
        file = YAML::LoadFile(_filename);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return satelliteVector;
    }

    if (file["satellites"] && file["satellites"].IsSequence()) {
        YAML::Node satellites{file["satellites"]};
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
    unsigned int anomalyGiven = 0;
    if (node["semi-major-axis"] && node["eccentricity"] && node["inclination"]
        && node["longitude-of-the-ascending-node"] && node["argument-of-periapsis"]) {
        auto a = node["semi-major-axis"].as<double>();
        auto e = node["eccentricity"].as<double>();
        auto i = node["inclination"].as<double>();
        auto W = node["longitude-of-the-ascending-node"].as<double>();
        auto w = node["argument-of-periapsis"].as<double>();

        if (node["true-anomaly"]) {
            auto TA = node["true-anomaly"].as<double>();
            satelliteBuilder.setKeplerianElementsTA(a, e, i, W, w, TA);
            anomalyGiven += 1;
        }
        if (node["mean-anomaly"]) {
            auto MA = node["mean-anomaly"].as<double>();
            satelliteBuilder.setKeplerianElementsMA(a, e, i, W, w, MA);
            anomalyGiven += 1;
        }
        if (node["eccentric-anomaly"]) {
            auto EA = node["eccentric-anomaly"].as<double>();
            satelliteBuilder.setKeplerianElementsEA(a, e, i, W, w, EA);
            anomalyGiven += 1;
        }

        if (anomalyGiven == 0) {
            throw std::invalid_argument{"You have to give at least one of the following orbital Elements:"
                                        "Eccentric Anomaly, Mean Anomaly or True Anomaly"};
        } else if (anomalyGiven > 1) {
            std::cerr << "You have given the satellite multiple anomalies. The program just uses one anomaly because"
                        << "the others can be derived. The program priorities the anomalies in the following order"
                        << "Eccentric Anomaly > Mean Anomaly > True Anomaly";
        }
    } else {
        throw std::invalid_argument{"The Keplerian Elements are not fully given!"};
    }
}
