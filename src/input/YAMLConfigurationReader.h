#pragma once

#include <utility>
#include <exception>
#include "input/InputConfigurationSource.h"
#include "input/OutputConfigurationSource.h"
#include "input/YAMLDataReader.h"
#include "input/TLESatcatDataReader.h"
#include "output/CSVWriter.h"
#include "output/CSVPatternWriter.h"
#include "output/VTKWriter.h"
#include "spdlog/spdlog.h"

/**
 * Reads the breakup simulation configuration data from an YAML file.
 */
class YAMLConfigurationReader : public InputConfigurationSource, public OutputConfigurationSource {

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
     * @return double
     * @throws a runtime_error if not specified
     */
    double getMinimalCharacteristicLength() const override;

    /**
     * Returns explicitly the type to use for the simulation. The Data Input should than have the corresponding
     * reasonable amount of satellites (EXPLOSION --> 1 satellite, COLLISION --> 2 satellites).
     * @attention If not given UNKNOWN is returned and the type will be derived from the number of satellites
     * @return SimulationType
     */
    SimulationType getTypeOfSimulation() const override;

    /**
    * Returns the current Maximal Given (NORAD-Catalog) ID. This is later required for the breakup simulation
    * to determine the IDs of the fragments. The nullopt is returned if this is not given.
    * @return std::optional<size_t>
    */
    std::optional<size_t> getCurrentMaximalGivenID() const override;

    /**
     * Returns an DataSource which has the ability to return an vector of satellites.
     * @return DataSource as shared pointer if it is wished to (re-)use in an object-oriented purpose
     * @throws an Exception if not given because not data == no simulation possible
     */
    std::shared_ptr<const DataSource> getDataReader() const override;

    /**
     * Returns the ID selection for the filter if given. Else an empty optional is returned.
     * @return filter or empty
     */
    std::optional<std::set<size_t>> getIDFilter() const override;

    /**
     * Reads in the which Output is wished by the YAML file.
     * @return a vector containing the Outputs according to the YAML file
     */
    std::vector<std::shared_ptr<OutputWriter>> getOutputTargets() const override;

};

