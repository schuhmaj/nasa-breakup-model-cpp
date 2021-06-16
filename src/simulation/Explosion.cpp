#include "Explosion.h"

void Explosion::generateFragments() {
    //Gets the one satellite from the input
    Satellite &sat = _input.at(0);

    //Sets the maximalCharacteristicLength which will be required later
    _maximalCharacteristicLength = sat.getCharacteristicLength();

    //The fragment Count, respectively Equation 2
    double fragmentCount = 6 * std::pow(_minimalCharacteristicLength, -1.6);

    _output.resize(static_cast<size_t>(fragmentCount));
}

void Explosion::characteristicLengthDistribution() {
    //The pdf for Explosions is: 0.0132578/x^2.6
    Breakup::characteristicLengthDistribution(-2.6);
}

void Explosion::deltaVelocityDistribution() {

}