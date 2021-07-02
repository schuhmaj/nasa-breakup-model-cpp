#pragma once

#include <utility>
#include <exception>
#include "input/ConfigurationSource.h"
#include "input/YAMLDataReader.h"
#include "input/TLESatcatDataReader.h"

/**
 * Reads the breakup simulation configuration data from an YAML file.
 */
class YAMLConfigurationReader : public ConfigurationSource {

    const YAML::Node _file;

public:

    /**
     * Creates a new YAML Configuration Reader.
     * @param filename
     * @throws an exception if the file is malformed or cannot be loaded
     */
    explicit YAMLConfigurationReader(const std::string &filename)
        : _file{YAML::LoadFile(filename)} {}

    /**
     * Creates a new YAML Configuration Reader.
     * @param filename
     * @throws an exception if the file is malformed or cannot be loaded
     */
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
     * Returns an DataSource which has the ability to return an vector of satellites.
     * @return DataSource as shared pointer if it is wished to (re-)use in an object-oriented purpose
     * @throws an Exception if not given because not data == no simulation possible
     */
    std::shared_ptr<DataSource> getDataReader() override;

    /**
     * Returns the ID selection for the filter if given. Else an empty optional is returned.
     * @return filter or empty
     */
    std::optional<std::set<size_t>> getIDFilter() override;

    bool getOutputWithKepler() override;

};

