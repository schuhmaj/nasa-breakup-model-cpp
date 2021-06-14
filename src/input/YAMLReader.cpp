#include "YAMLReader.h"

std::vector<Satellite> YAMLReader::getSatelliteCollection() {
    std::vector<Satellite> satelliteVector{};
    SatelliteBuilder satelliteBuilder{};
    YAML::Node file{};

    try {
        file = YAML::LoadFile(_filename);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return satelliteVector;
    }

    if (file["satellites"]) {
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

Satellite YAMLReader::parseSatellite(SatelliteBuilder &satelliteBuilder, const YAML::Node& node) {
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
    return satelliteBuilder.getResult();
}
