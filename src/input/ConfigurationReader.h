#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <map>
#include "yaml-cpp/yaml.h"
#include "input/DataReader.h"

/**
 * (Expressive) Return type for getTypeOfSimulation.
 */
enum class SimulationType {
    COLLISION, EXPLOSION, UNKNOWN
};

/**
 * Pure virtual Interface for Control Input.
 * Provides methods to get config data, like minimal L_c, type of simulation, maximal given NORAD-ID or
 * the source file(s) for Satellite Data.
 */
class ConfigurationReader {

public:

    inline const static std::map<std::string, SimulationType> stringToSimulationType{
            {"COLLISION", SimulationType::COLLISION},
            {"CO",       SimulationType::COLLISION},
            {"EXPLOSION", SimulationType::EXPLOSION},
            {"EX",       SimulationType::EXPLOSION}
    };

    virtual ~ConfigurationReader() = default;

    /**
     * Returns the minimal characteristic Length for fragments later created by the Breakup Simulation.
     * @return double
     */
    virtual double getMinimalCharacteristicLength() = 0;

    /**
     * Returns explicitly the type to use for the simulation. The Data Input should than have the corresponding
     * reasonable amount of satellites (EXPLOSION --> 1 satellite, COLLISION --> 2 satellites)
     * @return SimulationType
     */
    virtual SimulationType getTypeOfSimulation() = 0;

    /**
     * Returns the current Maximal Given (NORAD-Catalog) ID. This is later required for the breakup simulation
     * to determine the IDs of the fragments.
     * @return size_t
     */
    virtual size_t getCurrentMaximalGivenID() = 0;

    /**
     * Returns an DataReader which has the ability to return an vector of satellites.
     * @return DataReader as shared pointer if it is wished to (re-)use in an object-oriented purpose
     */
    virtual std::shared_ptr<DataReader> getDataReader() = 0;

};
