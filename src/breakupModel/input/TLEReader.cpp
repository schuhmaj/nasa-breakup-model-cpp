#include "TLEReader.h"

const std::map<char, size_t> TLEReader::alpha5NumberingSchemeOffset{
        {' ', 0},
        {'0', 0},
        {'1', 10000},
        {'2', 20000},
        {'3', 30000},
        {'4', 40000},
        {'5', 50000},
        {'6', 60000},
        {'7', 70000},
        {'8', 80000},
        {'9', 90000},
        {'A', 100000},
        {'B', 110000},
        {'C', 120000},
        {'D', 130000},
        {'E', 140000},
        {'F', 150000},
        {'G', 160000},
        {'H', 170000},
        {'J', 180000},
        {'K', 190000},
        {'L', 200000},
        {'M', 210000},
        {'N', 220000},
        {'P', 230000},
        {'Q', 240000},
        {'R', 250000},
        {'S', 260000},
        {'T', 270000},
        {'U', 280000},
        {'V', 290000},
        {'W', 300000},
        {'X', 310000},
        {'Y', 320000},
        {'Z', 330000}
};

std::map<size_t, OrbitalElements> TLEReader::getMappingIDOrbitalElements() const {
    std::map<size_t, OrbitalElements> mapping{};

    std::ifstream fileStream{_filepath};
    std::string line2;

    while (std::getline(fileStream, line2)) {
        if (line2.rfind('2', 0) == 0) {
            auto pair = parseLineTwo(line2);
            mapping.insert(pair);
        }
    }

    return mapping;
}

std::pair<size_t, OrbitalElements> TLEReader::parseLineTwo(const std::string &line) const {
    size_t id;
    std::array<double, 6> tleData{};

    try {
        //ID
        char firstCharOfID = line.at(2);
        std::string restID = line.substr(3, 4);
        id = std::stoul(restID);
        id += alpha5NumberingSchemeOffset.at(firstCharOfID);
        //Mean Motion [rev/day]
        tleData[0] = std::stod(line.substr(52, 11));
        //Eccentricity
        tleData[1] = std::stod("0." + line.substr(26, 7));
        //Inclination [deg]
        tleData[2] = std::stod(line.substr(8, 8));
        //RAAN [deg]
        tleData[3] = std::stod(line.substr(17, 8));
        //Argument of Perigee [deg]
        tleData[4] = std::stod(line.substr(34, 8));
        //Mean Anomaly [deg]
        tleData[5] = std::stod(line.substr(43, 8));
    } catch (std::exception &e) {
        throw std::runtime_error{"The TLE file \"" + _filepath +
                                 "\" is malformed! Some Data could not be parsed correctly into valid numbers!\n"
                                 "The issue appeared in the following line:\n"
                                 + line
        };
    }

    OrbitalElementsFactory factory{};
    return std::make_pair(id, factory.createFromTLEData(tleData));
}
