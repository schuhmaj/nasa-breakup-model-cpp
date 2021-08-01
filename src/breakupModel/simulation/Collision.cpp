#include "Collision.h"

void Collision::generateFragments() {
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
    this->createFragments(fragmentCount, sat1.getPosition());
}

void Collision::characteristicLengthDistribution() {
    //The pdf for Collisions is: 0.0101914/(x^2.71)
    Breakup::characteristicLengthDistribution(-2.71);
}

void Collision::assignParent() {
    //The names of the fragments for a given parent
    const Satellite &bigSat = _input.at(0);
    const Satellite &smallSat = _input.at(1);
    auto debrisNameBigPtr = std::make_shared<const std::string>(bigSat.getName() + "-Collision-Fragment");
    auto debrisNameSmallPtr = std::make_shared<const std::string>(smallSat.getName() + "-Collision-Fragment");

    //First specify the total mass
    double inputMass = bigSat.getMass() + smallSat.getMass();
    double resultMass = 0;
    auto satIt = _output.begin();
    for (; satIt != _output.end() && resultMass < inputMass; ++satIt) {
        resultMass += satIt->getMass();
    }
    //Erase potential "too-much" mass
    resultMass = resultMass > inputMass ? resultMass - (satIt - 1)->getMass() : resultMass;
    _output.erase(satIt, _output.end());

    double assignedBigMass = 0;
    //All debris greater than the small satellite must origin from the big one
    for (auto &sat : _output) {
        if (sat.getCharacteristicLength() > smallSat.getCharacteristicLength()) {
            sat.setName(debrisNameBigPtr);
            sat.setVelocity(bigSat.getVelocity());
            assignedBigMass += sat.getMass();
        }
    }

    //Assign the rest with respect to the already assigned debris-mass for the big satellite
    for (auto &sat : _output) {
        if (sat.getCharacteristicLength() <= smallSat.getCharacteristicLength()
            && assignedBigMass < bigSat.getMass() * resultMass / inputMass) {
            sat.setName(debrisNameBigPtr);
            sat.setVelocity(bigSat.getVelocity());
            assignedBigMass += sat.getMass();
        } else {
            sat.setName(debrisNameSmallPtr);
            sat.setVelocity(smallSat.getVelocity());
        }
    }
}

void Collision::deltaVelocityDistribution() {
    //Equation 12 mu = 0.9 * chi + 2.9
    Breakup::deltaVelocityDistribution(0.9, 2.9);
}
