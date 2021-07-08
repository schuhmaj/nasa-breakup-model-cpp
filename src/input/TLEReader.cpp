#include "TLEReader.h"


std::map<size_t, std::array<double, 6>> TLEReader::getMappingIDKepler() const {
    std::map<size_t, std::array<double, 6>> mapping{};

    std::ifstream fileStream{_filepath};

    while (!fileStream.eof()) {
        auto pair = getTwoLine(fileStream);
        mapping.insert(pair);
    }

    return mapping;
}

std::pair<size_t, std::array<double, 6>> TLEReader::getTwoLine(std::istream &istream) const {
    size_t id;
    std::array<double, 6> keplerElements{};
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
        keplerElements[0] = std::stod(line.substr(52, 11));
        //Eccentricity
        keplerElements[1] = std::stod("0." + line.substr(26, 7));
        //Inclination [deg]
        keplerElements[2] = std::stod(line.substr(8, 8));
        //RAAN [deg]
        keplerElements[3] = std::stod(line.substr(17, 8));
        //Argument of Perigee [deg]
        keplerElements[4] = std::stod(line.substr(34, 8));
        //Mean Anomaly [deg]
        keplerElements[5] = std::stod(line.substr(43, 8));
    } catch (std::exception &e) {
        throw std::runtime_error{"The TLE file " + _filepath + " is malformed! Some Data could not be parsed correctly"
                                                               "into valid numbers"};
    }

    return std::make_pair(id, keplerElements);
}
