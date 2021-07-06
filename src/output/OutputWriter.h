#pragma once

#include <vector>
#include <string>
#include "model/Satellite.h"
#include "simulation/Breakup.h"

/**
 * Interface for Output (Pure virtual).
 */
class OutputWriter {

public:

    OutputWriter() = default;

    virtual ~OutputWriter() = default;

    /**
     * Prints the Satellites to an output source.
     * @param satelliteCollection
     */
    virtual void printResult(const std::vector<Satellite> &satelliteCollection) = 0;

    /**
     * Prints the result Satellites to an output source.
     * Default implemented.
     * @param breakup
     */
    virtual void printResult(const Breakup &breakup) {
        this->printResult(breakup.getResult());
    }

};
