#include "Collision.h"

void Collision::generateFragments() {
    using util::operator-, util::euclideanNorm;
    //Get the two satellites from the input
    Satellite& sat1 = _input.at(0);
    Satellite& sat2 = _input.at(1);

    //Contains the mass M (later filled with an adequate value)
    double mass = 0;

    //Assume sat1 is always the bigger one
    if (sat1.getCharacteristicLength() < sat2.getCharacteristicLength()) {
        std::swap(sat1, sat2);
    }

    //The Relative Collision Velocity
    double dv = euclideanNorm(sat1.getVelocity() - sat2.getVelocity());

    //Calculate the Catastrophic Ratio, if greater than 40 J/kg then we have an catastrophic collision
    double catastrophicRatio = (sat2.getMass() * dv * dv) / (2 * sat1.getMass()*1000);
    if (catastrophicRatio < 40) {
        _isCatastrophic = false;
        mass = sat2.getMass() * dv;
    } else {
        _isCatastrophic = true;
        mass = sat1.getMass() + sat2.getMass();
    }

    //The fragment Count, respectively Equation 4
    double fragmentCount = 0.1 * std::pow(mass, 0.75) * std::pow(_minimalCharacteristicLength, -1.71);

    _output.resize(static_cast<size_t>(fragmentCount));
}

void Collision::characteristicLengthDistribution() {

}

void Collision::deltaVelocityDistribution() {

}
