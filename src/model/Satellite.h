#pragma once

#include <array>
#include <map>
#include <string>
#include <ostream>
#include <utility>
#include <cmath>
#include "util/UtilityKepler.h"
#include "util/UtilityContainer.h"

/**
 * Type of a Satellite
 * (Derived from the NORAD Catalog types)
 */
enum class SatType {
    SPACECRAFT, ROCKET_BODY, DEBRIS, UNKNOWN
};

std::ostream &operator<<(std::ostream &os, SatType satType);


/**
 * Represent an object in orbit.
 */
class Satellite {

    /**
     * The ID of the Satellite, must be unique e. g. the NORAD Catalog number.
     * @attention This parameter is a must!
     */
    size_t _id{0};

    /**
     * The name of the Satellite, more human readable.
     * If not given the string will be empty.
     */
    std::string _name{};

    /**
     * The type of the Satellite. Needed for determining the right equations for breakup.
     * If not given this will be always a SPACECRAFT.
     * @note For the simulation the two types SPACECRAFT (--> e.g. active satellites)
     * and ROCKET_BODY (--> e.g. upper stages) make a difference in what equation is utilised.
     * Furthermore there are the types DEBRIS and UNKNOWN which currently have no special effect.
     */
    SatType _satType{SatType::SPACECRAFT};

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
     * The area A [m^2] respectively the radar cross section (RCS) of the satellite
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

    /**
     * Map containing a mapping from String to SatType.
     */
    const static std::map<std::string, SatType> stringToSatType;

    /**
     * Map containing a mapping from SatType to String.
     */
    const static std::map<SatType, std::string> satTypeToString;

    Satellite() = default;

    explicit Satellite(size_t id)
            : _id{id} {}

    Satellite(std::string name, SatType satType)
            : _name{std::move(name)},
              _satType{satType} {}

    /**
     * Calculates the cartesian velocity and cartesian position of this satellite by using the Keplerian Elements.
     * This function sets the internal members _velocity and _position to the corresponding values.
     * @param a - semir-major axis [m]
     * @param e - eccentricity
     * @param i - inclination [rad]
     * @param W - longitude of the ascending node (big omega) [rad]
     * @param w - argument of periapsis (small omega) [rad]
     * @param EA - eccentric Anomaly [rad]
     * @note Code taken and adapted
     * from pykep (https://github.com/esa/pykep/blob/master/include/keplerian_toolbox/core_functions/par2ic.hpp)
     * [23.06.2021]
     */
    void keplerToCartesianEA(double a, double e, double i, double W, double w, double EA);

    /**
     * Calculates the cartesian velocity and cartesian position of this satellite by using the Keplerian Elements.
     * This function sets the internal members _velocity and _position to the corresponding values.
     * @param a - semir-major axis [m]
     * @param e - eccentricity
     * @param i - inclination [rad]
     * @param W - longitude of the ascending node (big omega) [rad]
     * @param w - argument of periapsis (small omega) [rad]
     * @param MA - mean Anomaly [rad]
     */
    void keplerToCartesianMA(double a, double e, double i, double W, double w, double MA);

    /**
     * Calculates the cartesian velocity and cartesian position of this satellite by using the Keplerian Elements.
     * This function sets the internal members _velocity and _position to the corresponding values.
     * @param a - semir-major axis [m]
     * @param e - eccentricity
     * @param i - inclination [rad]
     * @param W - longitude of the ascending node (big omega) [rad]
     * @param w - argument of periapsis (small omega) [rad]
     * @param TA - true Anomaly [rad]
     */
    void keplerToCartesianTA(double a, double e, double i, double W, double w, double TA);

    /**
     * Calculates the Keplerian Elements by using the satellite's caretsian position and velocity vectors.
     * @return an array consisting of the six Keplerian Elements in the following order [a, e, i, W, w, EA] where
     * a = semi-major-axis [m]; e = eccentricity; i = inclination [rad]; W = longitude of the ascending node [rad];
     * w = argument of periapsis [rad]; EA = eccentric Anomaly [rad]
     * @note Code taken and adapted from pykep
     * (https://github.com/esa/pykep/blob/master/include/keplerian_toolbox/core_functions/ic2par.hpp) [25.06.2021]
     */
    std::array<double, 6> cartesianToKeplerEA();

    /**
    * Calculates the Keplerian Elements by using the satellite's caretsian position and velocity vectors.
    * @return an array consisting of the six Keplerian Elements in the following order [a, e, i, W, w, MA] where
    * a = semi-major-axis [m]; e = eccentricity; i = inclination [rad]; W = longitude of the ascending node [rad];
    * w = argument of periapsis [rad]; MA = mean Anomaly [rad]
    */
    std::array<double, 6> cartesianToKeplerMA();

    /**
    * Calculates the Keplerian Elements by using the satellite's caretsian position and velocity vectors.
    * @return an array consisting of the six Keplerian Elements in the following order [a, e, i, W, w, TA] where
    * a = semi-major-axis [m]; e = eccentricity; i = inclination [rad]; W = longitude of the ascending node [rad];
    * w = argument of periapsis [rad]; TA = true Anomaly [rad]
    */
    std::array<double, 6> cartesianToKeplerTA();

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
        return not(lhs == rhs);
    }

    friend std::ostream &operator<<(std::ostream &os, const Satellite &satellite);

    /*
     * Getter and Setter
     */

    size_t getId() const {
        return _id;
    }

    void setId(size_t id) {
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

