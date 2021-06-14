#pragma once

#include <fstream>
#include <exception>
#include "yaml-cpp/yaml.h"
#include "InputReader.h"
#include "model/Satellite.h"
#include "model/SatelliteCollection.h"
#include "model/SatelliteBuilder.h"

/**
 * Reads Satellites from an YAML file.
 */
class YAMLReader : public InputReader {

public:

    using InputReader::InputReader;

    /**
     * Returns a SatelliteCollection. Satellites are read from the YAML file.<br>
     *
     * If the file name is malformed or the file is corrupt in other ways,
     * the returned SatelliteCollection will contain no satellites.<br>
     *
     * If the file exists, but some satellites are invalid then the SatelliteCollection will only contain valid
     * satellites! A valid satellite is a satellites which has every date needed to run the simulation with it.<br>
     * @return a SatelliteCollection
     */
    SatelliteCollection getSatelliteCollection() override;

private:

    /**
     * Parses a satellite from a given YAMl Node.
     * @param satelliteBuilder - an reference to an SatelliteBuilder
     * @param node - an reference to an YAML Node
     * @return a new Satellite
     * @throws an invalid_argument exception if the satellite is invalid
     */
    static Satellite parseSatellite(SatelliteBuilder &satelliteBuilder, const YAML::Node& node);

};


