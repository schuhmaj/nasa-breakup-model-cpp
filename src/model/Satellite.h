#pragma once

/**
 * Represent an object in orbit.
 * This object can either be ACTIVE (working satellite) or INACTIVE (dysfunctional satellite OR Debris)
 * TODO: Subclassing or Enumeration for status or is it required at all
 */
class Satellite {

    /**
     * The characteristic Length L_c
     */
    double _size;

    /**
     * The area-to-mass ratio A/M
     */
    double _areaToMassRatio;

    /**
     * The mass m
     */
    double _mass;

    /**
     * TODO: Vector or Scalar?
     * The velocity
     */
    double _velocity;




public:

    /**
     * Initialize a satellite with "empty" values (all set to zero).
     */
    Satellite();

};

