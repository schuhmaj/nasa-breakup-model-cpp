#pragma once

#include <utility>
#include <vector>
#include <array>
#include <cmath>
#include <random>
#include <algorithm>
#include "model/Satellite.h"
#include "util/UtilityContainer.h"
#include "util/UtilityFunctions.h"
#include "util/UtilityAreaMassRatio.h"

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
     * Member which is required to choose the correct Distribution function for the A/M values.
     */
    SatType _satType{SatType::SPACECRAFT};

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

protected:

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
     * Contains the specific part, so this method is for each subclass different.
     * @note This method is implemented in the subclasses and calls the general form with parameters in the base class
     */
    virtual void characteristicLengthDistribution() = 0;

    /**
     * Creates the Size Distribution according to an specific powerLaw Exponent.
     * The Exponent comes from the probability density function (pdf).
     * @attention Implemented in the base class, called by the subclasses with their parameters
     * @param powerLawExponent - double
     */
    virtual void characteristicLengthDistribution(double powerLawExponent);

    /**
     * Creates for every satellite the area-to-mass ratio according to Equation 6.
     */
    virtual void areaToMassRatioDistribution();

    /**
     * Implements the Delta Velocity Distribution. Assigns every satellite an cartesian velocity vector.
     * Therefore it first calculates the velocity according to Equation 11/ 12 as an scalar and transform this
     * into an cartesian vector.
     * @note This method is implemented in the subclasses and calls the general form with parameters in the base class
     */
    virtual void deltaVelocityDistribution() = 0;

    /**
     * Implements the Delta Velocity Distribution according to Equation 11/ 12.
     * The parameters can be described as the following: mu = factor * chi + offset where mu is the mean value of the
     * normal distribution.
     * @attention Implemented in the base class, called by the subclasses with their parameters
     * @param factor - for the chi
     * @param offset - on top of the chi
     */
    virtual void deltaVelocityDistribution(double factor, double offset);

private:

    /**
     * Calculates an A/M Value for a given L_c.
     * The utilised equation is chosen based on L_c and the SatType attribute of this Breakup.
     * @param characteristicLength in [m]
     * @param generator a generator "to power" the normal distributions
     * @return A/M value
     */
    double calculateAM(double characteristicLength, std::mt19937 &generator);

    std::array<double, 3> calculateVelocityVector(double velocity, std::mt19937 &generator);

};


