#include "Explosion.h"
#include "Breakup.h"

void Explosion::generateFragments() {
    //Gets the one satellite from the input
    Satellite &sat = _input.at(0);

    //Sets the maximalCharacteristicLength which will be required later
    _maximalCharacteristicLength = sat.getCharacteristicLength();

    //Sets the _satType attribute to the correct type (later required for the A/M)
    //The Default of this member is SPACECRAFT
    _satType = sat.getSatType();


    //The fragment Count, respectively Equation 2
    double fragmentCount = 6 * std::pow(_minimalCharacteristicLength, -1.6);

    const std::string debrisName{sat.getName() + "-Explosion-Fragment"};
    this->createFragments(fragmentCount, debrisName);
}

void Explosion::characteristicLengthDistribution() {
    //The pdf for Explosions is: 0.0132578/x^2.6
    Breakup::characteristicLengthDistribution(-2.6);
}

void Explosion::deltaVelocityDistribution() {
    //Equation 11 mu = 0.2 * chi + 1.85
    Breakup::deltaVelocityDistribution(0.2, 1.85);
}