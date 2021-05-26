#pragma once

#include "model/SatelliteCollection.h"

/**
 * Interface for Output
 */
class OutputWriter {

    OutputWriter() = default;

    virtual ~OutputWriter() = default;

    virtual void printResult(SatelliteCollection &satelliteCollection) = 0;

};
