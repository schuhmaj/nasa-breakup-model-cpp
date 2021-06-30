#include "Satellite.h"

std::ostream &operator<<(std::ostream &os, const Satellite &satellite) {
    //TODO Rework later
    return os << "ID:\t" << satellite._id << "\n"
              << "Name:\t" << satellite._name << "\n"
              << "L_c:\t" << satellite._characteristicLength << "\n"
              << "M:\t" << satellite._mass << "\n"
              << "A/M:\t" << satellite._areaToMassRatio << "\n";
}
