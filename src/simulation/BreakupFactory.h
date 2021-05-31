#pragma once

#include <memory>
#include "input/InputReader.h"
#include "output/OutputWriter.h"
#include "Breakup.h"
#include "Explosion.h"
#include "Collision.h"

/**
 * Interface to easily create a Breakup Event.
 * Based on a modified Builder Pattern.
 */
class BreakupFactory {

    std::shared_ptr<InputReader> _inputReader;

public:

    /**
     * Adds an input source for the satellites.
     * @param inputReader - a shared pointer to an input source
     * @return this
     */
    BreakupFactory &changeInputSource(const std::shared_ptr<InputReader> &inputReader);


    Breakup getExplosion();

    Breakup getCollision();


};
