#pragma once

#include <utility>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include "model/Satellite.h"
#include "util/UtilityContainer.h"
#include "util/UtilityFunctions.h"

/**
 * Pure virtual class which needs a Collection of Satellites as input and output and simulates a breakup
 * which is either a collision or an explosion.
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
     * The maximal characteristic length. The value is set during the run of the simulation and derived
     * from the input satellites (the max L_c of them).
     */
    double _maximalCharacteristicLength{};

    /**
     * Contains the input satellites. Normally the fragmentCount for this collection is either one (explosion) or
     * two (collision)
     */
    std::vector<Satellite> _input;

    /**
     * Contains the output satellites aka fragments of the collision or explosion
     */
    std::vector<Satellite> _output;

public:

    Breakup() = default;

    explicit Breakup(std::vector<Satellite> &input)
            : _input{input},
              _output{} {}

    explicit Breakup(std::vector<Satellite> &&input)
            : _input{input},
              _output{} {}

    virtual ~Breakup() = default;

    /**
     * Runs the simulation.
     * Afterwards one can get the result with the member method getResult().
     */
    virtual void run();

    /**
     * Return the result of the breakup event
     * @return SatelliteCollection with the generated fragments
     */
    std::vector<Satellite> &getResult() {
        return _output;
    }

private:

    /**
     * This method contains every step required to re-run the simulation.
     * The method for example resets the _output's size to zero.
     */
    virtual void init();

    /**
     * Creates the a number of fragments, following the Equation 2 for Explosions and
     * Equation 4 for Collisions.
     */
    virtual void generateFragments() = 0;

    /**
     * Creates the Size Distribution. After the fragments are generated this method will assign
     * every fragment a L_c value based on the corresponding power law distribution (Equation 2 and 4).
     */
    virtual void characteristicLengthDistribution() = 0;

    virtual void areaToMassRatioDistribution();

    virtual void deltaVelocityDistribution() = 0;

};


