#include "CSVPatternWriter.h"

const std::map<char, std::function<void(const Satellite &sat, std::stringstream &stream)>> CSVPatternWriter::functionMap{
        {'I', [](const Satellite &sat, std::stringstream &stream) -> void { stream << sat.getId(); }},
        {'n', [](const Satellite &sat, std::stringstream &stream) -> void { stream << sat.getName(); }},
        {'t', [](const Satellite &sat, std::stringstream &stream) -> void { stream << sat.getSatType(); }},
        {'L', [](const Satellite &sat, std::stringstream &stream) -> void { stream << sat.getCharacteristicLength(); }},
        {'R', [](const Satellite &sat, std::stringstream &stream) -> void { stream << sat.getAreaToMassRatio(); }},
        {'A', [](const Satellite &sat, std::stringstream &stream) -> void { stream << sat.getArea(); }},
        {'m', [](const Satellite &sat, std::stringstream &stream) -> void { stream << sat.getMass(); }},
        {'v', [](const Satellite &sat, std::stringstream &stream) -> void {
            using util::operator<<;
            stream << sat.getVelocity();
        }},
        {'p', [](const Satellite &sat, std::stringstream &stream) -> void {
            using util::operator<<;
            stream << sat.getPosition();
        }},
        {'a', [](const Satellite &sat, std::stringstream &stream) -> void { stream << sat.getKeplerEA()[0]; }},
        {'e', [](const Satellite &sat, std::stringstream &stream) -> void { stream << sat.getKeplerEA()[1]; }},
        {'i', [](const Satellite &sat, std::stringstream &stream) -> void { stream << sat.getKeplerEA()[2]; }},
        {'W', [](const Satellite &sat, std::stringstream &stream) -> void { stream << sat.getKeplerEA()[3]; }},
        {'w', [](const Satellite &sat, std::stringstream &stream) -> void { stream << sat.getKeplerEA()[4]; }},
        {'M', [](const Satellite &sat, std::stringstream &stream) -> void { stream << sat.getKeplerMA()[5]; }},
        {'E', [](const Satellite &sat, std::stringstream &stream) -> void { stream << sat.getKeplerEA()[5]; }},
        {'T', [](const Satellite &sat, std::stringstream &stream) -> void { stream << sat.getKeplerTA()[5]; }}
};

const std::map<char, std::string> CSVPatternWriter::headerMap{
        {'I', "ID"},
        {'n', "Name"},
        {'t', "Satellite Type"},
        {'L', "Characteristic Length [m]"},
        {'R', "A/M [m^2/kg]"},
        {'A', "Area [m^2]"},
        {'m', "Mass [kg]"},
        {'v', "Velocity [m/s]"},
        {'p', "Position [m]"},
        {'a', "Semi-Major-Axis [m]"},
        {'e', "Eccentricity"},
        {'i', "Inclination [rad]"},
        {'W', "Longitude of the ascending node [rad]"},
        {'w', "Argument of periapsis [rad]"},
        {'M', "Mean Anomaly [rad]"},
        {'E', "Eccentric Anomaly [rad]"},
        {'T', "True Anomaly [rad]"}
};

void CSVPatternWriter::printResult(const std::vector<Satellite> &satelliteCollection) {
    //Header
    std::stringstream header{};
    for (auto headerIt = _myHeader.begin(); headerIt != _myHeader.end() - 1; ++headerIt) {
        header << *headerIt << ',';
    }
    header << _myHeader.back();
    _logger->info(header.str());

    //CSV Lines
    for (const auto &sat : satelliteCollection) {
        std::stringstream stream{};
        for (auto funIt = _myToDo.begin(); funIt != _myToDo.end() - 1; ++funIt) {
            (*funIt)(sat, stream);
            stream << ',';
        }
        _myToDo.back()(sat, stream);
        _logger->info(stream.str());
    }
}
