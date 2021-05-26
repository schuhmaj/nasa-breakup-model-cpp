#pragma once

#include "model/SatelliteCollection.h"

/**
 * Interface for Input.
 * TODO Subclass from it, in order of priority:
 * - XML Input? For direct v, r elements, etc --> More versatile
 * - TLE/ SatCat as second alternative? Or integrate into XML as "alternative"
 */
class InputReader {

public:

    InputReader() = default;

    virtual ~InputReader() = default;

    virtual SatelliteCollection getSatelliteCollection() = 0;

};

