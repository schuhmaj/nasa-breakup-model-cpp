#pragma once

#include <utility>
#include <exception>
#include "input/ConfigurationReader.h"
#include "input/YAMLDataReader.h"

/**
 * Reads the breakup simulation configuration data from an YAML file.
 */
class YAMLConfigurationReader : public ConfigurationReader {

    const YAML::Node _file;

public:

    explicit YAMLConfigurationReader(const std::string &filename)
        : _file{YAML::LoadFile(filename)} {}

    explicit YAMLConfigurationReader(std::string &&filename)
            : _file{YAML::LoadFile(filename)} {}


    /**
     * Returns the minimal characteristic Length for fragments later created by the Breakup Simulation.
     * @attention If not given by the YAML, 0.05 [m] is returned as default value.
     * @return double
     */
    double getMinimalCharacteristicLength() override;

    /**
     * Returns explicitly the type to use for the simulation. The Data Input should than have the corresponding
     * reasonable amount of satellites (EXPLOSION --> 1 satellite, COLLISION --> 2 satellites).
     * @attention If not given UNKNOWN is returned and the type will be derived from the number of satellites
     * @return SimulationType
     */
    SimulationType getTypeOfSimulation() override;

    /**
    * Returns the current Maximal Given (NORAD-Catalog) ID. This is later required for the breakup simulation
    * to determine the IDs of the fragments.
    * @attention If not given 0 is returned as default value (might lead to problems in the afternoon)
    * @return size_t
    */
    size_t getCurrentMaximalGivenID() override;

    /**
     * Returns an DataReader which has the ability to return an vector of satellites.
     * @return DataReader as shared pointer if it is wished to (re-)use in an object-oriented purpose
     * @throws an Exception if not given because not data == no simulation possible
     */
    std::shared_ptr<DataReader> getDataReader() override;

};

