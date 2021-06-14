#include "Collision.h"

void Collision::generateFragments() {

    //TODO Resume here

    _isCatastrophic = false;

    double mass = 0;

    double fragmentCount = 0.1 * std::pow(mass, 0.75) * std::pow(_minimalCharacteristicLength, -1.71);

    _output.resize(static_cast<size_t>(fragmentCount));
}

void Collision::characteristicLengthDistribution() {

}

void Collision::deltaVelocityDistribution() {

}
