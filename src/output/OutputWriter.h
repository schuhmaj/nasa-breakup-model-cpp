#pragma once

#include <vector>
#include "model/Satellite.h"

/**
 * Interface for Output
 * TODO: Maybe start already writing during calculation as second thread?
 * To be evaluated
 */
class OutputWriter {

    OutputWriter() = default;

    virtual ~OutputWriter() = default;

    virtual void printResult(std::vector<Satellite> &satelliteCollection) = 0;

};
