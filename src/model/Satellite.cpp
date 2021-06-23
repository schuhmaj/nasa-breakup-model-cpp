#include "Satellite.h"

std::ostream &operator<<(std::ostream &os, SatType satType) {
    return os << Satellite::satTypeToString.at(satType);
}

const std::map<std::string, SatType> Satellite::stringToSatType{{"SPACECRAFT",  SatType::SPACECRAFT},
                                                                {"SC",          SatType::SPACECRAFT},
                                                                {"ROCKET_BODY", SatType::ROCKET_BODY},
                                                                {"RB",          SatType::ROCKET_BODY},
                                                                {"DEBRIS",      SatType::DEBRIS},
                                                                {"DEB",         SatType::DEBRIS},
                                                                {"UNKNOWN",     SatType::UNKNOWN}
};

const std::map<SatType, std::string> Satellite::satTypeToString{{SatType::SPACECRAFT,  "SPACECRAFT"},
                                                                {SatType::ROCKET_BODY, "ROCKET_BODY"},
                                                                {SatType::DEBRIS,      "DEBRIS"},
                                                                {SatType::UNKNOWN,     "UNKNOWN"}
};

std::ostream &operator<<(std::ostream &os, const Satellite &satellite) {
    //TODO Rework later
    return os << "ID:\t" << satellite._id << "\n"
              << "Name:\t" << satellite._name << "\n"
              << "L_c:\t" << satellite._characteristicLength << "\n"
              << "M:\t" << satellite._mass << "\n"
              << "A/M:\t" << satellite._areaToMassRatio << "\n";
}
