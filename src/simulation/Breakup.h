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
     * The minimal characteristic length in [m]
     * The Breakup Simulation will only produce fragments greater or equal this fragmentCount.
     */
    const double _minimalCharacteristicLength{0.05};

    /**
     * The maximal characteristic length in [m]
     * The value is set during the run of the simulation and derived
     * from the input satellites (the max L_c of them).
     */
    double _maximalCharacteristicLength{};

    /**
     * The Breakup simulation will assign each satellite, created after setting this, an ID greater than this number.
     * ID will be unique if _currentMaxGivenID is set to the current maximal given NORAD-Catalog ID.
     * @note Default value is zero which could led to confusion because those numbers directly after zero are already
     * assigned to existing satellites. Nevertheless the simulation never uses those IDs, so this value is only a
     * MUST if the users wants to work with valid satellite IDs beyond the Breakup simulation.
     */
    size_t _currentMaxGivenID{0};

    /**
     * Member which is required to choose the correct Distribution function for the A/M values.
     * This value is set during the run of the simulation.
     */
    SatType _satType{SatType::SPACECRAFT};

    /**
     * The random number generator, used by the implementation.
     * It is default initialized with a random seed when the Breakup is constructed.
     */
    std::mt19937 _randomNumberGenerator{std::random_device{}()};

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

    explicit Breakup(std::vector<Satellite> input)
            : _input{std::move(input)},
              _output{} {}

    Breakup(std::vector<Satellite> input, double minimalCharacteristicLength)
            : _input{std::move(input)},
              _minimalCharacteristicLength{minimalCharacteristicLength} {};

    Breakup(std::vector<Satellite> input, double minimalCharacteristicLength, size_t currentMaxGivenID)
            : _input{std::move(input)},
              _minimalCharacteristicLength{minimalCharacteristicLength},
              _currentMaxGivenID{currentMaxGivenID} {}


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
    std::vector<Satellite> getResult() const {
        return _output;
    }

    /**
     * This method allows to set the seed of the random number generator to generate predictable fragments when
     * the run method is called.
     * @param seed
     * @return this
     */
    virtual Breakup &setSeed(unsigned long seed = std::random_device{}());

protected:

    /**
     * This method contains every step required to re-run the simulation.
     * The method for example resets the _output's size to zero.
     */
    virtual void prepare();

    /**
     * Creates the a number of fragments, following the Equation 2 for Explosions and
     * Equation 4 for Collisions.
     */
    virtual void generateFragments() = 0;

    /**
     * Actually creates the fragments (Resizes the vector and assigns a unique ID and name to each fragment)
     * Further the position vector is correctly set. This one is derived from the parents.
     * @param fragmentCount - the number of fragments which should be created
     * @param debrisName - the name for the fragments
     * @param position - position of the fragment, derived from the one parent (explosion) or from first parent (collision)
     */
    virtual void
    createFragments(size_t fragmentCount, const std::string &debrisName, const std::array<double, 3> &position);

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
     * @return A/M value
     */
    double calculateAM(double characteristicLength);

    /**
     * Transforms a scalar velocity into a 3-dimensional cartesian velocity vector.
     * The transformation is based around a uniform Distribution.
     * @param velocity - scalar velocity
     * @return 3-dimensional cartesian velocity vector
     */
    std::array<double, 3> calculateVelocityVector(double velocity);

public:

    double getMinimalCharacteristicLength() const {
        return _minimalCharacteristicLength;
    }

    double getMaximalCharacteristicLength() const {
        return _maximalCharacteristicLength;
    }

    size_t getCurrentMaxGivenId() const {
        return _currentMaxGivenID;
    }

};


