#include "SatelliteBuilder.h"


const std::map<std::string, SatType> SatelliteBuilder::stringToSatType{{"SPACECRAFT",  SPACECRAFT},
                                                                       {"SC",  SPACECRAFT},
                                                                       {"ROCKET_BODY", ROCKET_BODY},
                                                                       {"RB", ROCKET_BODY},
                                                                       {"DEBRIS",      DEBRIS},
                                                                       {"DEB",      DEBRIS},
                                                                       {"UNKNOWN",     UNKNOWN}
};

SatelliteBuilder &SatelliteBuilder::reset() {
    _satellite = Satellite{};
    _hasID = false;
    _hasMass = false;
    _hasVelocity = false;
    _hasPosition = false;
    return *this;
}

SatelliteBuilder &SatelliteBuilder::setID(unsigned long id) {
    _satellite.setId(id);
    _hasID = true;
    return *this;
}

SatelliteBuilder &SatelliteBuilder::setName(const std::string &name) {
    _satellite.setName(name);
    return *this;
}

SatelliteBuilder &SatelliteBuilder::setName(std::string &&name) {
    _satellite.setName(name);
    return *this;
}

SatelliteBuilder &SatelliteBuilder::setSatType(SatType satType) {
    _satellite.setSatType(satType);
    return *this;
}

SatelliteBuilder &SatelliteBuilder::setSatType(const std::string &satType) {
    try {
        this->setSatType(stringToSatType.at(satType));
    } catch (std::exception &e) {
        throw std::invalid_argument{"Satellite Type is not well defined!"};
    }
    return *this;
}

SatelliteBuilder &SatelliteBuilder::setSatType(std::string &&satType) {
    try {
        this->setSatType(stringToSatType.at(satType));
    } catch (std::exception &e) {
        throw std::invalid_argument{"Satellite Type is not well defined!"};
    }
    return *this;
}

SatelliteBuilder &SatelliteBuilder::setMass(double mass) {
    _satellite.setMass(mass);
    _satellite.setArea(-1);
    _satellite.setCharacteristicLength(-1);
    _hasMass = true;
    return *this;
}

SatelliteBuilder &SatelliteBuilder::setMassByArea(double area) {
    double radius = calculateCircleRadius(area);
    double mass = calculateSphereMass(radius);
    _satellite.setMass(mass);
    _satellite.setArea(area);
    _satellite.setCharacteristicLength(radius);
    _hasMass = true;
    return *this;
}

SatelliteBuilder &SatelliteBuilder::setVelocity(const std::array<double, 3> &velocity) {
    //TODO Similiar handling to mass? Or integrate Keplerian Elements into Satellite Model?
    _satellite.setVelocity(velocity);
    _hasVelocity = true;
    return *this;
}

SatelliteBuilder &SatelliteBuilder::setPosition(const std::array<double, 3> &position) {
    _satellite.setPosition(position);
    _hasPosition = true;
    return *this;
}

SatelliteBuilder &SatelliteBuilder::setKeplerianElements() {
    //TODO Insert pykep Utilisation here to convert Keplerian Elements to Velocity vector
    _hasVelocity = true;
    _hasPosition = true;
    return *this;
}

Satellite &SatelliteBuilder::getResult() {
    if (!_hasID) {
        throw std::invalid_argument{"Satellite has no ID!"};
    }
    if (!_hasMass) {
        throw std::invalid_argument{"Satellite has no mass or way to derive the mass!"};
    }
    if (!_hasVelocity) {
        throw std::invalid_argument{"Satellite has no velocity or way to derive the velocity!"};
    }
    return _satellite;
}
