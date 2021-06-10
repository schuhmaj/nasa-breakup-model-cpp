#pragma once

#include <array>

/**
 * Type of a Satellite
 * (Derived from the NORAD Catalog types)
 */
enum class SatType {
    SPACECRAFT, ROCKET_BODY, DEBRIS, UNKNOWN
};


/**
 * Represent an object in orbit.
 * This object can either be ACTIVE (working satellite) or INACTIVE (dysfunctional satellite OR Debris)
 */
class Satellite {

    /**
     * The ID of the Satellite, must be unique
     * @note e. g. the NORAD Catalog number
     */
    size_t _id;

    /**
     * The type of the Satellite
     * @note Needed for determining the right equations for breakup
     */
    SatType _satType;

    /**
     * The characteristic Length L_c [m]
     */
    double _size;

    /**
     * The area-to-mass ratio A/M [m^2/kg]
     */
    double _areaToMassRatio;

    /**
     * The mass m [kg]
     */
    double _mass;

    /**
     * The area A [m^2]
     * @note Determined by either input (Radar data) or by the L_c (with Equation 8/9)
     */
    double _area;

    /**
     * The cartesian velocity vector v [m/s^2]
     * @note Determined with the Keplerian elements from the input or directly given
     */
    std::array<double, 3> _velocity;

    /**
     * The cartesian position vector [m]
     * @note Determined with the Keplerian elements from the input or directly given
     */
    std::array<double, 3> _position;

    /* TODO
     * Implement Keplerian Elements --> v, r
     * Not necessarily required for the simulation (only in case of non-catastrophic collision)
     */


public:

    /**
     * Generates a new Satellite with empty (zero-set) values.
     * TODO Currently unhappy - maybe use a factory? Definitely to refactor, when input is implemented
     */
    Satellite(size_t id = 0, SatType satType = SatType::SPACECRAFT, double size = 0.05, double area = 0)
        : _id{id},
          _satType{satType},
          _size{size},
          _areaToMassRatio{0},
          _mass{0},
          _area{area},
          _velocity{std::array<double, 3>{0, 0, 0}},
          _position{std::array<double, 3>{0, 0, 0}} {}


    /**
     * Compares two Satellites by comparing their IDs.
     * @param a - Satellite
     * @param b - Satellite
     * @return true if thy have the same ID
     */
    friend bool operator==(const Satellite &a, const Satellite &b) {
        return a._id == b._id;
    }


    /**
     * Compares two Satellites by comparing their IDs.
     * @param a - Satellite
     * @param b - Satellite
     * @return true if they do not have the same ID
     */
    friend bool operator!=(const Satellite &a, const Satellite &b) {
        return a._id != b._id;
    }

};
