#pragma once

#include <string>
#include <vector>
#include "model/Satellite.h"

/**
 * Pure virtual Interface for Data Input.
 * Provides methods to get satellites from an specific input file.
 */
class DataReader {

public:

    virtual ~DataReader() = default;

    /**
     * Returns a satellite collection. Input form varies depending on subclass
     * @return SatelliteCollection
     */
    virtual std::vector<Satellite> getSatelliteCollection() = 0;

};

