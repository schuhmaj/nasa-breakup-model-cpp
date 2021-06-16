#include "Breakup.h"

#include <iostream>

void Breakup::run() {
    //1. Step: Remove all old stuff which there possible is and makes the Breakup ready to go
    this->init();

    //2. Step: Generate the new Satellites
    this->generateFragments();

    //3. Step: Assign every new Satellite a value for L_c
    this->characteristicLengthDistribution();

    //4. Step: Calculate the A/M (area-to-mass-ratio), A (area) and M (mass) values for every Satellite
    this->areaToMassRatioDistribution();

    //5. Step: Calculate the Ejection velocity for every Satellite
    this->deltaVelocityDistribution();

}

void Breakup::init() {
    _output.clear();
}

void Breakup::characteristicLengthDistribution(double powerLawExponent) {
    using util::transformUniformToPowerLaw;
    std::random_device rd;
    std::mt19937 generator{rd()};
    std::uniform_real_distribution<> uniformRealDistribution{0.0, 1.0};

    std::for_each(_output.begin(),
                  _output.end(),
                  [&](Satellite &sat) {
                      double y = uniformRealDistribution(generator);
                      double L_c = transformUniformToPowerLaw(_minimalCharacteristicLength,
                                                              _maximalCharacteristicLength,
                                                              powerLawExponent,
                                                              y);
                      sat.setCharacteristicLength(L_c);
                  });
}

void Breakup::areaToMassRatioDistribution() {
    //TODO No distinction between Collision and Explosion

}
