#pragma once

#include <array>

/**
 * Represent an object in orbit.
 * This object can either be ACTIVE (working satellite) or INACTIVE (dysfunctional satellite OR Debris)
 * TODO: Subclassing or Enumeration for status or is it required at all
 */
class Satellite {

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
    double _area

    /**
     * The cartesian velocity vector v [m/s^2]
     * @note Determined with the Keplerian elements from the input or directly given
     */
    std::array<double, 3> _velocity;

    /**
     * The cartesian position vector [m]
     * @note Determined with the Keplerian elements from the input or directly given
     */
    std::array<double, 3> _position

    /* TODO
     * Implement Keplerian Elements --> v, r
     * Not necessarily required for the simulation (only in case of non-catastrophic collision)
     */


public:

    Satellite() = default;

};

