#include "TLEReader.h"


std::map<size_t, std::array<double, 6>> TLEReader::getMappingIDKepler() {
    std::map<size_t, std::array<double, 6>> mapping{};

    std::ifstream fileStream{_filename};

    while (!fileStream.eof()) {
        auto pair = getTwoLine(fileStream);
        mapping.insert(pair);
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
    } while (line.rfind('2', 0) != 0);

    //ID
    id = std::stoul(line.substr(2, 5));
    //Mean Motion
    keplerElements[0] = std::stod(line.substr(52, 11));
    //Eccentricity
    keplerElements[1] = std::stod("0." + line.substr(26, 7));
    //Inclination
    keplerElements[2] = std::stod(line.substr(8, 8));
    //RAAN
    keplerElements[3] = std::stod(line.substr(17, 8));
    //Argument of Perigee
    keplerElements[4] = std::stod(line.substr(34, 8));
    //Mean Anomaly
    keplerElements[5] = std::stod(line.substr(43, 8));

    return std::make_pair(id, keplerElements);
}
