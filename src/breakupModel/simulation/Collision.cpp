#include "Collision.h"

void Collision::calculateFragmentCount() {
    using util::operator-, util::euclideanNorm;
    //Get the two satellites from the input
    Satellite &sat1 = _input.at(0);
    Satellite &sat2 = _input.at(1);

    //Sets the maximalCharacteristicLength which will be required later
    _maximalCharacteristicLength = std::max(sat1.getCharacteristicLength(), sat2.getCharacteristicLength());

    //Sets the _satType attribute to the correct type (later required for the A/M)
    //The Default of this member is SPACECRAFT
    if (sat1.getSatType() == SatType::ROCKET_BODY || sat2.getSatType() == SatType::ROCKET_BODY) {
        _satType = SatType::ROCKET_BODY;
    }

    //Sets the _input mass which will be required later for mass conservation purpose
    _inputMass = sat1.getMass() + sat2.getMass();

    //Contains the mass M (later filled with an adequate value)
    double mass = 0;

    //Assume sat1 is always the bigger one
    if (sat1.getCharacteristicLength() < sat2.getCharacteristicLength()) {
        std::swap(sat1, sat2);
    }

    //The Relative Collision Velocity
    double dv = euclideanNorm(sat1.getVelocity() - sat2.getVelocity());

    //Calculate the Catastrophic Ratio, if greater than 40 J/kg then we have an catastrophic collision
    //A catastrophic collision means that both satellites are fully fragmented whereas in a non-catastrophic collision
    //only the smaller satellite is fragmented (see here Section: Collision in [johnson et al.]
    double catastrophicRatio = (sat2.getMass() * dv * dv) / (2 * sat1.getMass() * 1000);
    if (catastrophicRatio < 40) {
        _isCatastrophic = false;
        mass = sat2.getMass() * dv;
    } else {
        _isCatastrophic = true;
        mass = sat1.getMass() + sat2.getMass();
    }

    //The fragment Count, respectively Equation 4
    auto fragmentCount = static_cast<size_t>(0.1 * std::pow(mass, 0.75) *
                                             std::pow(_minimalCharacteristicLength, -1.71));
    this->generateFragments(fragmentCount, sat1.getPosition());
}

void Collision::characteristicLengthDistribution() {
    //The pdf for Collisions is: 0.0101914/(x^2.71)
    Breakup::characteristicLengthDistribution(-2.71);
}

void Collision::assignParentProperties() {
    //The names of the fragments for a given parent
    const Satellite &bigSat = _input.at(0);
    const Satellite &smallSat = _input.at(1);
    auto debrisNameBigPtr = std::make_shared<const std::string>(bigSat.getName() + "-Collision-Fragment");
    auto debrisNameSmallPtr = std::make_shared<const std::string>(smallSat.getName() + "-Collision-Fragment");

    //Assign debris the big parent if they are greater than the small parent
    double assignedMassForBigSatellite = 0;

    auto lcIt = _output._characteristicLength.begin();
    auto mIt = _output._mass.begin();
    auto nameIt = _output._name.begin();
    auto vIt = _output._velocity.begin();
    for (; lcIt != _output._characteristicLength.end(); ++lcIt, ++nameIt, ++vIt, ++mIt) {
        if (*lcIt > smallSat.getCharacteristicLength()) {
            *nameIt = debrisNameBigPtr;
            *vIt = bigSat.getVelocity();
            assignedMassForBigSatellite += *mIt;
        }
    }
    //Assign the rest with respect to the already assigned debris-mass for the big satellite
    //first if: the mass of the bigSat is normed to the actual produced mass of the simulation
    lcIt = _output._characteristicLength.begin();
    mIt = _output._mass.begin();
    nameIt = _output._name.begin();
    vIt = _output._velocity.begin();
    const double normedMassBigSat = bigSat.getMass() * _outputMass / _inputMass;
    for (; lcIt != _output._characteristicLength.end(); ++lcIt, ++nameIt, ++vIt, ++mIt) {
        if (*lcIt <= smallSat.getCharacteristicLength()
            && assignedMassForBigSatellite < normedMassBigSat) {
            *nameIt = debrisNameBigPtr;
            *vIt = bigSat.getVelocity();
            assignedMassForBigSatellite += *mIt;
        } else {
            *nameIt = debrisNameSmallPtr;
            *vIt = smallSat.getVelocity();
        }
    }
}

void Collision::deltaVelocityDistribution() {
    //Equation 12 mu = 0.9 * chi + 2.9
    Breakup::deltaVelocityDistribution(0.9, 2.9);
}
