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
    using util::operator<<;
    _logger->info(
            "ID,Name,Satellite Type,Characteristic Length [m],A/M [m^2/kg],Area [m^2],Mass [kg],"
            "Velocity [m/s],Position [m]");
    for (const auto &sat : satelliteCollection) {
        _logger->info("{},{},{},{},{},{},{},{},{}", sat.getId(), sat.getName(), sat.getSatType(),
                      sat.getCharacteristicLength(), sat.getAreaToMassRatio(), sat.getArea(), sat.getMass(),
                      sat.getVelocity(), sat.getPosition());
    }
}

void CSVWriter::printKepler(const std::vector<Satellite> &satelliteCollection) const {
    using util::operator<<;
    _logger->info("ID,Name,Satellite Type,Characteristic Length [m],A/M [m^2/kg],Area [m^2],Mass [kg],"
                  "Velocity [m/s],Position [m],"
                  "Semi-Major-Axis [m],Eccentricity,Inclination [rad],Longitude of the ascending node [rad],"
                  "Argument of periapsis [rad],Mean Anomaly [rad]");
    for (const auto &sat : satelliteCollection) {
        auto kepler = sat.getKeplerMA();
        _logger->info("{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}", sat.getId(), sat.getName(), sat.getSatType(),
                      sat.getCharacteristicLength(), sat.getAreaToMassRatio(), sat.getArea(), sat.getMass(),
                      sat.getVelocity(), sat.getPosition(),
                      kepler[0], kepler[1], kepler[2], kepler[3], kepler[4], kepler[5]);
    }
}

void CSVWriter::csvLine(std::ostream &ostream, const Satellite &satellite) {
    using util::operator<<;
    ostream.precision(17);
    ostream << satellite.getId() << ','
            << satellite.getName() << ','
            << satellite.getSatType() << ','
            << satellite.getCharacteristicLength() << ','
            << satellite.getAreaToMassRatio() << ','
            << satellite.getArea() << ','
            << satellite.getMass() << ','
            << satellite.getVelocity() << ','
            << satellite.getPosition();
    if (_withKepler) {
        auto kepler = satellite.getKeplerMA();
        ostream << ","
                << kepler[0] << ','
                << kepler[1] << ','
                << kepler[2] << ','
                << kepler[3] << ','
                << kepler[4] << ','
                << kepler[5];
    }
    ostream << "\n";
}

void CSVWriter::csvHeader(std::ostream &ostream) const {
    ostream << "ID" << ','
            << "Name" << ','
            << "Satellite Type" << ','
            << "Characteristic Length [m]" << ','
            << "A/M [m^2/kg]" << ','
            << "Area [m^2]" << ','
            << "Mass [kg]" << ','
            << "Velocity [m/s]" << ','
            << "Position [m]";
    if (_withKepler) {
        ostream << ","
                << "Semi-Major-Axis [m]" << ','
                << "Eccentricity" << ','
                << "Inclination [rad]" << ','
                << "Longitude of the ascending node [rad]" << ','
                << "Argument of periapsis [rad]" << ','
                << "Mean Anomaly [rad]";
    }
    ostream << "\n";
}
