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
    double catastrophicRatio = (sat2.getMass() * dv * dv) / (2 * sat1.getMass() * 1000);
    if (catastrophicRatio < 40) {
        _isCatastrophic = false;
        mass = sat2.getMass() * dv;
    } else {
        _isCatastrophic = true;
        mass = sat1.getMass() + sat2.getMass();
    }

    //The fragment Count, respectively Equation 4
    double fragmentCount = 0.1 * std::pow(mass, 0.75) * std::pow(_minimalCharacteristicLength, -1.71);

    const std::string debrisName{sat1.getName() + " " + sat2.getName() + "-Collision-Fragment"};
    this->createFragments(fragmentCount, debrisName);
}

void Collision::characteristicLengthDistribution() {
    //The pdf for Collisions is: 0.0101914/(x^2.71)
    Breakup::characteristicLengthDistribution(-2.71);
}

void Collision::deltaVelocityDistribution() {
    //Equation 12 mu = 0.9 * chi + 2.9
    Breakup::deltaVelocityDistribution(0.9, 2.9);
}
