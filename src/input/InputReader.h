#pragma once

#include "model/SatelliteCollection.h"

/**
 * Interface for Input.
 */
class InputReader {

public:

    InputReader() = default;

    virtual ~InputReader() = default;

    virtual SatelliteCollection getSatelliteCollection() = 0;

};

