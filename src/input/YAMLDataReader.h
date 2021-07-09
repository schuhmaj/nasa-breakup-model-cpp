#pragma once

#include <fstream>
#include <exception>
#include <vector>
#include <array>
#include <utility>
#include <iostream>
#include "yaml-cpp/yaml.h"
#include "input/DataSource.h"
#include "model/Satellite.h"
#include "model/SatelliteBuilder.h"
#include "model/OrbitalElementsFactory.h"

/**
 * Reads Satellites from an YAML file.
 */
class YAMLDataReader : public DataSource {

    /**
     * The root node of the YAML file
     */
    const YAML::Node _file;

public:

    /**
     * Creates a new YAML Data Reader.
     * @param filename
     * @throws an exception if the file is malformed or cannot be loaded
     */
    explicit YAMLDataReader(const std::string &filename)
            : _file{YAML::LoadFile(filename)} {}

    /**
    * Creates a new YAML Data Reader.
    * @param filename
    * @throws an exception if the file is malformed or cannot be loaded
    */
    explicit YAMLDataReader(std::string &&filename)
            : _file{YAML::LoadFile(filename)} {}

    /**
     * Returns a SatelliteCollection. Satellites are read from the YAML file.<br>
     *
     * If the file name is malformed or the file is corrupt in other ways,
     * the returned SatelliteCollection will contain no satellites.<br>
     *
     * If the file exists, but some satellites are invalid then the SatelliteCollection will only contain valid
     * satellites! A valid satellite is a satellites which has every date needed to run the simulation with it.<br>
     * @return a SatelliteCollection
     * @throws a runtime_error if a satellite is incomplete
     */
    std::vector<Satellite> getSatelliteCollection() const override;

private:

    /**
     * Parses a satellite from a given YAML Node.
     * @param satelliteBuilder - an reference to an SatelliteBuilder
     * @param node - an reference to an YAML Node
     * @return a new Satellite
     * @throws an runtime_error exception if the satellite is invalid
     */
    static Satellite parseSatellite(SatelliteBuilder &satelliteBuilder, const YAML::Node &node);

    /**
     * Parses the Keplerian Elements from a YAML Node.
     * @param satelliteBuilder - an reference to an SatelliteBuilder
     * @param node - an reference to an YAML Node, containing the Keplerian Elements
     * @note This method is used by the parseSatellite(SatelliteBuilder&, YAML::Node&) method
     * @throws a runtime_error if Kepler elements not fully given
     */
    static void parseKepler(SatelliteBuilder &satelliteBuilder, const YAML::Node &node);

};


