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
        double a = node["semi-major-axis"].as<double>();
        double e = node["eccentricity"].as<double>();
        double i = node["inclination"].as<double>();
        double W = node["longitude-of-the-ascending-node"].as<double>();
        double w = node["argument-of-periapsis"].as<double>();

        if (node["eccentric-anomaly"]) {
            double EA = node["eccentric-anomaly"].as<double>();
            OrbitalElements orbitalElements{a, e, std::make_pair(i, AngularUnit::RADIAN),
                                            std::make_pair(W, AngularUnit::RADIAN),
                                            std::make_pair(w, AngularUnit::RADIAN),
                                            std::make_tuple(EA, AngularUnit::RADIAN, OrbitalAnomalyType::ECCENTRIC)};

            satelliteBuilder.setOrbitalElements(orbitalElements);
        } else if (node["mean-anomaly"]) {
            double MA = node["mean-anomaly"].as<double>();
            OrbitalElements orbitalElements{a, e, std::make_pair(i, AngularUnit::RADIAN),
                                            std::make_pair(W, AngularUnit::RADIAN),
                                            std::make_pair(w, AngularUnit::RADIAN),
                                            std::make_tuple(MA, AngularUnit::RADIAN, OrbitalAnomalyType::MEAN)};

            satelliteBuilder.setOrbitalElements(orbitalElements);
        } else if (node["true-anomaly"]) {
            double TA = node["true-anomaly"].as<double>();
            OrbitalElements orbitalElements{a, e, std::make_pair(i, AngularUnit::RADIAN),
                                            std::make_pair(W, AngularUnit::RADIAN),
                                            std::make_pair(w, AngularUnit::RADIAN),
                                            std::make_tuple(TA, AngularUnit::RADIAN, OrbitalAnomalyType::TRUE)};

            satelliteBuilder.setOrbitalElements(orbitalElements);
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
