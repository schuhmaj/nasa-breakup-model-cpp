#pragma once

#include "input/DataSource.h"
#include "input/CSVReader.h"

/**
 * Class which reads data from a tle.txt and a satcat.csv
 */
class TLESatcatDataReader : public DataSource {

    /**
     * Delegation to read the satcat.csv
     */
    CSVReader _csvReader;

public:

    /**
     * Returns the a SatelliteCollection by reading the given satcat.csv and TLE data.
     * Neither of the two of them contains all necessary information. So this method also merges the information
     * by using the unique ID of each satellite.
     * @return a Collection of Satellites
     */
    std::vector<Satellite> getSatelliteCollection() override;

};

