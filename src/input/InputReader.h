#pragma once

#include <string>
#include "model/SatelliteCollection.h"

/**
 * Interface for Input. Provides methods to get satellites from an specific input file.
 */
class InputReader {

protected:

    const std::string _filename;

public:

    explicit InputReader(const std::string &filename)
            : _filename{filename} {}

    explicit InputReader(std::string &&filename)
            : _filename{filename} {}

    virtual ~InputReader() = default;

    /**
     * Returns a satellite collection. Input form varies depending on subclass
     * @return SatelliteCollection
     */
    virtual SatelliteCollection getSatelliteCollection() = 0;

};

