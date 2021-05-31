#pragma once

#include <string>
#include "model/SatelliteCollection.h"

/**
 * Interface for Input.
 * TODO Subclass from it, in order of priority:
 * - YAML Input? For direct v, r elements, etc --> More versatile
 * - TLE/ SatCat as second alternative? Or integrate into YAML as "alternative"
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

    virtual SatelliteCollection getSatelliteCollection() = 0;

};

