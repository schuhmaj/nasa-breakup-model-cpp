#pragma once

#include <fstream>
#include <exception>
#include "yaml-cpp/yaml.h"
#include "InputReader.h"
#include "model/Satellite.h"
#include "model/SatelliteCollection.h"
#include "model/SatelliteBuilder.h"

class YAMLReader : public InputReader {

public:

    using InputReader::InputReader;

    SatelliteCollection getSatelliteCollection() override;

private:

    static Satellite parseSatellite(SatelliteBuilder &satelliteBuilder, const YAML::Node& node);

};


