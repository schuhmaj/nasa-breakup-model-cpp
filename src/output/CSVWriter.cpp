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
            << satellite.getPosition()
            << "\n";
}

void CSVWriter::csvHeader(std::ostream &ostream) {
    ostream << "ID" << ','
            << "Name" << ','
            << "Satellite Type" << ','
            << "Characteristic Length [m]" << ','
            << "A/M [m^2/kg]" << ','
            << "Area [m^2]" << ','
            << "Mass [kg]" << ','
            << "Velocity [m/s]" << ','
            << "Position [m]"
            << "\n";
}
