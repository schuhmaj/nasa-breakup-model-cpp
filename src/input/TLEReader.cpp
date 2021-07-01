#include "TLEReader.h"


std::map<size_t, std::array<double, 6>> TLEReader::getMappingIDKepler() {
    std::map<size_t, std::array<double, 6>> mapping{};

    std::ifstream fileStream{_filepath};

    try {
        while (!fileStream.eof()) {
            auto pair = getTwoLine(fileStream);
            mapping.insert(pair);
        }
    } catch (std::exception &e) {
        throw;
    }

    return mapping;
}

std::pair<size_t, std::array<double, 6>> TLEReader::getTwoLine(std::istream &istream) {
    size_t id;
    std::array<double, 6> keplerElements{};
    std::string line{};

    //Goto Line 2
    do {
        std::getline(istream, line);
        if (line.empty()) {
            throw std::runtime_error{"Malformed TLE file! There was one empty line!"};
        }
    } while (line.rfind('2', 0) != 0);

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

    return std::make_pair(id, keplerElements);
}
