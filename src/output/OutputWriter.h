#pragma once

#include <vector>
#include <string>
#include "model/Satellite.h"

/**
 * Interface for Output.
 */
class OutputWriter {

    const std::vector<Satellite> _satelliteCollection;

public:

    explicit OutputWriter(const std::vector<Satellite> &satelliteCollection)
            : _satelliteCollection{satelliteCollection} {};

    virtual ~OutputWriter() = default;

    /**
     * Virtual method which prints the internal satellite Collection to an output source.
     * The Implementation is dependent on the corresponding subclass.
     */
    virtual void printResult() = 0;

};
