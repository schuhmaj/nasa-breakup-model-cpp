#include "Breakup.h"

#include <iostream>

void Breakup::run() {
    int fragmentCount = this->fragmentCount();

    _output = SatelliteCollection{fragmentCount};


}

void Breakup::areaToMassRatioDistribution() {
    //TODO No distinction between Collision and Explosion

}
