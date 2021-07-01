#pragma once

#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include "model/Satellite.h"

/**
 * Interface for Input. Provides methods to get satellites from an specific input file.
 */
class InputReader {

protected:

    const std::string _filename;

public:

    explicit InputReader(std::string filename)
            : _filename{std::move(filename)} {}

    virtual ~InputReader() = default;

    /**
     * Returns a satellite collection. Input form varies depending on subclass
     * @return SatelliteCollection
     */
    virtual std::vector<Satellite> getSatelliteCollection() = 0;

};

