#pragma once

#include <array>
#include <string>
#include <ostream>

/**
 * Type of a Satellite
 * (Derived from the NORAD Catalog types)
 */
enum SatType {
    SPACECRAFT, ROCKET_BODY, DEBRIS, UNKNOWN
};


/**
 * Represent an object in orbit.
 * This object can either be ACTIVE (working satellite) or INACTIVE (dysfunctional satellite OR Debris)
 */
class Satellite {

    /*
     * Naming Members
     */

    /**
     * The ID of the Satellite, must be unique e. g. the NORAD Catalog number.
     * @attention This parameter is a must!
     */
    unsigned long _id{};

    /**
     * The name of the Satellite, more human readable.
     * If not given the string will be empty.
     */
    std::string _name{};

    /**
     * The type of the Satellite. Needed for determining the right equations for breakup.
     * If not given this will be always a SPACECRAFT.
     */
    SatType _satType{SPACECRAFT};

    /**
     * The characteristic Length L_c [m]
     * @remark Determined by the breakup simulation
     */
    double _characteristicLength{};

    /**
     * The area-to-mass ratio A/M [m^2/kg]
     * @remark Determined by the breakup simulation
     */
    double _areaToMassRatio{};

    /**
     * The mass m [kg]
     * @note Determined by input parameter (if mass is well-known)
     * @note Derived of input parameter by using Equation 1 by using the radius [m]
     *       calculated with the radar cross section [m^2]
     * @remark Determined by the breakup simulation by using the area A and the area-to-mass ratio A/M
     */
    double _mass{};

    /**
     * The area A [m^2]
     * @note Determined by input parameter (radar cross section, part of satcat)
     * @remark Determined by the breakup simulation by using the characteristic length L_c (Equation 8/9)
     */
    double _area{};

    /**
     * The cartesian velocity vector v [m/s^2]
     * @note Directly given as input parameter
     * @note Derived from the Keplerian elements
     * @remark Determined by the breakup simulation
     */
    std::array<double, 3> _velocity{};

    /**
     * The cartesian position vector [m]
     * @note Directly given as input parameter
     * @note Derived from the Keplerian elements
     */
    std::array<double, 3> _position{};


    /* TODO Subclass to add Keplerian Elements only if they are given, no memory overhead or Factory/ Builder before?
     * Implement Keplerian Elements --> v, r
     * Not necessarily required for the simulation (only in case of non-catastrophic collision)
     */

public:

    Satellite() = default;

    explicit Satellite(unsigned long id)
        : _id{id} {}


    /**
     * Compares two Satellites by comparing their IDs.
     * @param lhs - Satellite
     * @param rhs - Satellite
     * @return true if thy have the same ID
     */
    friend bool operator==(const Satellite &lhs, const Satellite &rhs) {
        return lhs._id == rhs._id;
    }


    /**
     * Compares two Satellites by comparing their IDs.
     * @param lhs - Satellite
     * @param rhs - Satellite
     * @return true if they do not have the same ID
     */
    friend bool operator!=(const Satellite &lhs, const Satellite &rhs) {
        return lhs._id != rhs._id;
    }

    friend std::ostream &operator<<(std::ostream &os, const Satellite &satellite);

    /*
     * Getter and Setter
     */

    unsigned long getId() const {
        return _id;
    }

    void setId(unsigned long id) {
        _id = id;
    }

    const std::string &getName() const {
        return _name;
    }

    void setName(const std::string &name) {
        _name = name;
    }

    SatType getSatType() const {
        return _satType;
    }

    void setSatType(SatType satType) {
        _satType = satType;
    }

    double getCharacteristicLength() const {
        return _characteristicLength;
    }

    void setCharacteristicLength(double characteristicLength) {
        _characteristicLength = characteristicLength;
    }

    double getAreaToMassRatio() const {
        return _areaToMassRatio;
    }

    void setAreaToMassRatio(double areaToMassRatio) {
        _areaToMassRatio = areaToMassRatio;
    }

    double getMass() const {
        return _mass;
    }

    void setMass(double mass) {
        _mass = mass;
    }

    double getArea() const {
        return _area;
    }

    void setArea(double area) {
        _area = area;
    }

    const std::array<double, 3> &getVelocity() const {
        return _velocity;
    }

    void setVelocity(const std::array<double, 3> &velocity) {
        _velocity = velocity;
    }

    const std::array<double, 3> &getPosition() const {
        return _position;
    }

    void setPosition(const std::array<double, 3> &position) {
        _position = position;
    }

};

