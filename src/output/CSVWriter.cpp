#include "CSVWriter.h"

void CSVWriter::printResult(const std::vector<Satellite> &satelliteCollection) {
    if (_withKepler) {
        this->printKepler(satelliteCollection);
    } else {
        this->printStandard(satelliteCollection);
    }
}

void CSVWriter::printResult(const Breakup &breakup) {
    this->printResult(breakup.getResult());
}

void CSVWriter::printStandard(const std::vector<Satellite> &satelliteCollection) const {
    _logger->info(
            "ID,Name,Satellite Type,Characteristic Length [m],A/M [m^2/kg],Area [m^2],Mass [kg],"
            "Velocity [m/s],Position [m]");
    for (const auto &sat : satelliteCollection) {
        auto &v = sat.getVelocity();
        auto &p = sat.getPosition();
        _logger->info("{},{},{},{},{},{},{},[{} {} {}],[{} {} {}]", sat.getId(), sat.getName(), sat.getSatType(),
                      sat.getCharacteristicLength(), sat.getAreaToMassRatio(), sat.getArea(), sat.getMass(),
                      v[0], v[1], v[2], p[0], p[1], p[2]);
    }
}

void CSVWriter::printKepler(const std::vector<Satellite> &satelliteCollection) const {
    using util::operator<<;
    _logger->info("ID,Name,Satellite Type,Characteristic Length [m],A/M [m^2/kg],Area [m^2],Mass [kg],"
                  "Velocity [m/s],Position [m],"
                  "Semi-Major-Axis [m],Eccentricity,Inclination [rad],Longitude of the ascending node [rad],"
                  "Argument of periapsis [rad],Mean Anomaly [rad]");
    for (const auto &sat : satelliteCollection) {
        auto &v = sat.getVelocity();
        auto &p = sat.getPosition();
        auto kepler = sat.getKeplerMA();
        _logger->info("{},{},{},{},{},{},{},[{} {} {}],[{} {} {}],{},{},{},{},{},{}", sat.getId(), sat.getName(), sat.getSatType(),
                      sat.getCharacteristicLength(), sat.getAreaToMassRatio(), sat.getArea(), sat.getMass(),
                      v[0], v[1], v[2], p[0], p[1], p[2],
                      kepler[0], kepler[1], kepler[2], kepler[3], kepler[4], kepler[5]);
    }
}
