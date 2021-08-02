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
    auto fragmentCount = static_cast<size_t>(6.0 * std::pow(_minimalCharacteristicLength, -1.6));
    this->createFragments(fragmentCount, sat.getPosition());
}

void Explosion::characteristicLengthDistribution() {
    //The pdf for Explosions is: 0.0132578/x^2.6
    Breakup::characteristicLengthDistribution(-2.6);
}

void Explosion::assignParentProperties() {
    //The name of the fragments
    const Satellite &parent = _input.at(0);
    auto debrisNamePtr = std::make_shared<const std::string>(parent.getName() + "-Explosion-Fragment");

    //Check the mass and assign name + base velocity
    const double inputMass = parent.getMass();
    double outputMass = 0;
    for (auto &sat : _output) {
        sat.setName(debrisNamePtr);
        sat.setVelocity(parent.getVelocity());
        outputMass += sat.getMass();
    }
    //Print a warning for the user if the _output contains more mass than the input
    if (inputMass < outputMass) {
        spdlog::warn("The Explosion of {} produced {} kg, but the input only contained {} kg",
                     parent.getName(), outputMass, inputMass);
    }
}

void Explosion::deltaVelocityDistribution() {
    //Equation 11 mu = 0.2 * chi + 1.85
    Breakup::deltaVelocityDistribution(0.2, 1.85);
}
