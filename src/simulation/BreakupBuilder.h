#pragma once

#include <memory>
#include "input/InputReader.h"
#include "output/OutputWriter.h"
#include "Breakup.h"

enum BreakupType {
    COLLISION, EXPLOSION, UNDEFINED
};

/**
 * Interface to easily create a Breakup Event.
 * Based on a modified Builder Pattern.
 */
class BreakupBuilder {

    std::shared_ptr<InputReader> _inputReader;

    BreakupType _breakupType;

public:

    /**
     * Defines the Breakup-Type (COLLISION or EXPLOSION)
     * @param breakupType to construct
     * @return this
     */
    BreakupBuilder &addBreakupType(BreakupType breakupType);

    /**
     * Adds an input source for the satellites.
     * @param inputReader - a shared pointer to an input source
     * @return this
     */
    BreakupBuilder &addInput(const std::shared_ptr<InputReader> &inputReader);

    /**
     * Returns a new Breakup "simulation" object.
     * @return result - Breakup
     */
    std::unique_ptr<Breakup> getResult();

};
