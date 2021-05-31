#pragma once

#include <fstream>
#include "yaml-cpp/yaml.h"
#include "InputReader.h"
#include "model/Satellite.h"
#include "model/SatelliteCollection.h"

class YAMLReader : public InputReader {

public:

    using InputReader::InputReader;

    SatelliteCollection getSatelliteCollection() override;

};


