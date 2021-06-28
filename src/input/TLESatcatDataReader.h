#pragma once

#include "input/DataSource.h"
#include "input/CSVReader.h"
#include "input/TLEReader.h"

/**
 * Class which reads data from a tle.txt and a satcat.csv
 */
class TLESatcatDataReader : public DataSource {

    /**
     * Delegation to read the satcat.csv
     */
    CSVReader _satcatReader;

    /**
     * Delegation to read the tle.txt
     */
    TLEReader _tleReader;

public:

    /**
     * Returns the a SatelliteCollection by reading the given satcat.csv and TLE data.
     * Neither of the two of them contains all necessary information. So this method also merges the information
     * by using the unique ID of each satellite.
     * @return a Collection of Satellites
     */
    std::vector<Satellite> getSatelliteCollection() override;

};

