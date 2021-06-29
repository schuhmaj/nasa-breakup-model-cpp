#include "TLESatcatDataReader.h"

std::vector<Satellite> TLESatcatDataReader::getSatelliteCollection() {
    auto satcat = _satcatReader.getLines();

    return std::vector<Satellite>();
}
