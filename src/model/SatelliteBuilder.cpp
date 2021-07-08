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
    _satellite.setVelocity(velocity);
    _hasVelocity = true;
    return *this;
}

SatelliteBuilder &SatelliteBuilder::setPosition(const std::array<double, 3> &position) {
    _satellite.setPosition(position);
    _hasPosition = true;
    return *this;
}

SatelliteBuilder &
SatelliteBuilder::setKeplerianElementsEA(const std::array<double, 6> &keplerianElements) {
    _hasVelocity = true;
    _hasPosition = true;
    _satellite.setCartesianByKeplerEA(keplerianElements);
    return *this;
}

SatelliteBuilder &
SatelliteBuilder::setKeplerianElementsMA(const std::array<double, 6> &keplerianElements) {
    _hasVelocity = true;
    _hasPosition = true;
    _satellite.setCartesianByKeplerMA(keplerianElements);
    return *this;
}

SatelliteBuilder &
SatelliteBuilder::setKeplerianElementsTA(const std::array<double, 6> &keplerianElements) {
    _hasVelocity = true;
    _hasPosition = true;
    _satellite.setCartesianByKeplerTA(keplerianElements);
    return *this;
}

SatelliteBuilder &SatelliteBuilder::setKeplerianElementsTLEFormat(const std::array<double, 6> &keplerianElements) {
    _hasVelocity = true;
    _hasPosition = true;
    _satellite.setCartesianByKeplerTLEFormat(keplerianElements);
    return *this;
}

Satellite &SatelliteBuilder::getResult() {
    if (!_hasID) {
        std::stringstream message{};
        message << _satellite << " has no valid ID!";
        throw std::runtime_error{message.str()};
    }
    if (!_hasMass) {
        std::stringstream message{};
        message << _satellite << " has no mass or way to derive the mass!";
        throw std::runtime_error{message.str()};
    }
    if (!_hasVelocity) {
        std::stringstream message{};
        message << _satellite << " has no velocity or way to derive the velocity!";
        throw std::runtime_error{message.str()};
    }
    if (!_hasPosition) {
        SPDLOG_INFO("{} has no position. This is not a problem!", _satellite);
    }
    return _satellite;
}
