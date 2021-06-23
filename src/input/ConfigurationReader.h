#pragma once

#include <string>
#include <memory>
#include "input/DataReader.h"

enum class SimulationType {
    COLLISION, EXPLOSION
};

/**
 * Pure virtual Interface for Control Input.
 * Provides methods to get config data, like minimal L_c, type of simulation, maximal given NORAD-ID or
 * the source file(s) for Satellite Data.
 */
class ConfigurationReader {

public:

    virtual ~ConfigurationReader() = default;

    virtual double getMinimalCharacteristicLength() = 0;

    virtual SimulationType getTypeOfSimulation() = 0;

    virtual size_t getCurrentMaximalGivenID() = 0;

    virtual std::shared_ptr<DataReader> getDataReader() = 0;

};
