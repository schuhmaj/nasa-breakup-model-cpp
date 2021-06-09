#pragma once

#include <utility>
#include "model/SatelliteCollection.h"

/**
 * Pure virtual class which needs a Collection of Satellites as input and output and simulates a breakup
 * which is either a collision or an explosion.
 * TODO: Idea is to use the Template Method Pattern to share common properties like A/M functions
 */
class Breakup {

protected:

    /**
     * The minimal characteristic length. The Breakup Simulation will only produce fragments
     * greater or equal this fragmentCount.
     * It is given in [m]
     * TODO Better integration program flow!!!!
     */
    const double _minimalCharacteristicLength{0.05};

    /**
     * Contains the input satellites. Normally the fragmentCount for this collection is either one (explosion) or
     * two (collision)
     */
    SatelliteCollection _input;

    /**
     * Contains the output satellites aka fragments of the collision or explosion
     * TODO: To be reviewed very later: just use one Collection for both input and output?
     */
    SatelliteCollection _output;

public:

    Breakup() = default;

    explicit Breakup(SatelliteCollection &input)
            : _input{input},
              _output{} {}

    explicit Breakup(SatelliteCollection &&input)
            : _input{input},
              _output{} {}

    virtual ~Breakup() = default;

    virtual void run();

    /**
     * Return the result of the breakup event
     * @return SatelliteCollection with the generated fragments
     */
    SatelliteCollection &getResult() {
        return _output;
    }

private:

    /**
     * Returns the number of fragments created by the Breakup.
     * @return fragments created
     */
    virtual int fragmentCount() = 0;

    /**
     * Returns the
     */
    virtual void sizeDistribution() = 0;

    virtual void areaToMassRatioDistribution();

    virtual void deltaVelocityDistribution() = 0;

};


