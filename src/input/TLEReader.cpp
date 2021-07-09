#include "TLEReader.h"


std::map<size_t, OrbitalElements> TLEReader::getMappingIDOrbitalElements() const {
    std::map<size_t, OrbitalElements> mapping{};

    std::ifstream fileStream{_filepath};

    while (!fileStream.eof()) {
        auto pair = getTwoLine(fileStream);
        mapping.insert(pair);
    }

    return mapping;
}

std::pair<size_t, OrbitalElements> TLEReader::getTwoLine(std::istream &istream) const {
    size_t id;
    double meanMotion, eccentricity, inclination, raan, argOfPer, ma;
    std::string line{};

    //Goto Line 2
    do {
        std::getline(istream, line);
        if (line.empty()) {
            throw std::runtime_error{"The TLE file " + _filepath + " is malformed! There was an empty line inside!"};
        }
    } while (line.rfind('2', 0) != 0);

    try {
        //ID
        id = std::stoul(line.substr(2, 5));
        //Mean Motion [rev/day]
        meanMotion = std::stod(line.substr(52, 11));
        //Eccentricity
        eccentricity = std::stod("0." + line.substr(26, 7));
        //Inclination [deg]
        inclination = std::stod(line.substr(8, 8));
        //RAAN [deg]
        raan = std::stod(line.substr(17, 8));
        //Argument of Perigee [deg]
        argOfPer = std::stod(line.substr(34, 8));
        //Mean Anomaly [deg]
        ma = std::stod(line.substr(43, 8));
    } catch (std::exception &e) {
        throw std::runtime_error{"The TLE file " + _filepath + " is malformed! Some Data could not be parsed correctly"
                                                               "into valid numbers"};
    }

    return std::make_pair(id, OrbitalElements{meanMotion, eccentricity, inclination, raan, argOfPer, ma});
}
