#include "CSVWriter.h"

void CSVWriter::printResult(const std::vector<Satellite> &satelliteCollection) {
    std::ofstream csvFile{_filename};
    if (csvFile.bad()) {
        throw std::ios_base::failure{"A problem occurred writing to the output file!"};
    } else {
        csvHeader(csvFile);
        for (auto &sat : satelliteCollection) {
            csvLine(csvFile, sat);
        }
        csvFile.close();
    }
}

void CSVWriter::printResult(const Breakup &breakup) {
    this->printResult(breakup.getResult());
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
