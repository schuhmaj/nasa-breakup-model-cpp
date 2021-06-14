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

void Breakup::areaToMassRatioDistribution() {
    //TODO No distinction between Collision and Explosion

}
