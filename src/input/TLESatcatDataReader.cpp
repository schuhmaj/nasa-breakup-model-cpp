#include "TLESatcatDataReader.h"

std::vector<Satellite> TLESatcatDataReader::getSatelliteCollection() {
    std::vector<Satellite> satellites{};
    SatelliteBuilder satelliteBuilder{};

    std::map<size_t, std::tuple<std::string, SatType, double>> mappingSatcat;
    std::map<size_t, std::array<double, 6>> mappingTLE;

    try {
        mappingSatcat = this->getSatcatMapping();
    } catch (std::exception &e) {
        std::cerr << e.what();
        std::cerr << "The satcat.csv file (or similar name) was not parsed correctly. There might be issues with the data!";
    }

    try {
        mappingTLE = _tleReader.getMappingIDKepler();
    } catch (std::exception &e) {
        std::cerr << e.what();
        std::cerr << "The tle.txt file (or similar name) was not parsed correctly. There might be issues with the data!";
    }

    satellites.reserve(std::max(mappingSatcat.size(), mappingTLE.size()));

    //We just search for satellites which appear in both mappings
    // --> No missing data possible (but not necessarily wrong information
    for (auto const& [id, keplerDataTLE] : mappingTLE) {
        try {
            if (mappingSatcat.count(id) != 0) {
                auto &dataSatcat = mappingSatcat[id];
                satellites.push_back(
                        satelliteBuilder
                                .reset()
                                .setID(id)
                                .setName(std::get<0>(dataSatcat))
                                .setSatType(std::get<1>(dataSatcat))
                                .setMassByArea(std::get<2>(dataSatcat))
                                .setKeplerianElementsTLEFormat(keplerDataTLE)
                                .getResult()
                );
            }
        } catch (std::exception &e) {
            std::cerr << e.what();
        }
    }
    return satellites;
}

std::map<size_t, std::tuple<std::string, SatType, double>> TLESatcatDataReader::getSatcatMapping() {
    std::map<size_t, std::tuple<std::string, SatType, double>> mapping{};

    //If the mapping should contain more infos --> Here's the code to change that
    auto satcat = _satcatReader.getLines();
    for (auto const& [name, identifier, id, type, statusCode, owner, launchDate, launchSite, decayDate, period,
                      inclination, apogee, perigee, rcs, dataStatusCode, orbitCenter, OrbitType] : satcat) {

        mapping.insert(std::make_pair(id, std::make_tuple(name, type, rcs)));
    }

    return mapping;
}
