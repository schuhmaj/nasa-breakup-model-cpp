#include "SatelliteBuilder.h"

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
        this->setSatType(Satellite::stringToSatType.at(satType));
    } catch (std::exception &e) {
        throw std::invalid_argument{"Satellite Type is not well defined!"};
    }
    return *this;
}

SatelliteBuilder &SatelliteBuilder::setSatType(std::string &&satType) {
    try {
        this->setSatType(Satellite::stringToSatType.at(satType));
    } catch (std::exception &e) {
        throw std::invalid_argument{"Satellite Type is not well defined!"};
    }
    return *this;
}

SatelliteBuilder &SatelliteBuilder::setMass(double mass) {
    //TODO To be reviewed!!!!!! Is this good or is it a very bad assumption?
    double characteristicLength = util::calculateCharacteristicLengthFromMass(mass);
    double area = util::calculateCircleArea(characteristicLength);
    _satellite.setMass(mass);
    _satellite.setArea(area);
    _satellite.setCharacteristicLength(characteristicLength);
    _hasMass = true;
    return *this;
}

SatelliteBuilder &SatelliteBuilder::setMassByArea(double area) {
    double characteristicLength = util::calculateCharacteristicLength(area);
    double mass = util::calculateSphereMass(characteristicLength);
    _satellite.setMass(mass);
    _satellite.setArea(area);
    _satellite.setCharacteristicLength(characteristicLength);
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
