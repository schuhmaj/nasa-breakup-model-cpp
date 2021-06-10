#pragma once

#include <memory>
#include "input/InputReader.h"
#include "output/OutputWriter.h"
#include "Breakup.h"
#include "Explosion.h"
#include "Collision.h"

/**
 * Interface to easily create a Breakup Simulation.
 */
class BreakupFactory {

    std::shared_ptr<InputReader> _inputReader;

public:

    explicit BreakupFactory(std::shared_ptr<InputReader> &inputReader)
            : _inputReader{inputReader} {}

    /**
     * Adds an input source for the satellites.
     * @param inputReader - a shared pointer to an input source
     * @return this
     */
    BreakupFactory &changeInputSource(const std::shared_ptr<InputReader> &inputReader);

    /**
     * Creates a new Explosion Breakup Simulation with the given input.
     * TODO Add error handling
     * @return Breakup Simulation
     */
    std::unique_ptr<Breakup> getExplosion() const;

    /**
     * Creates a new Collision Breakup Simulation with the given input.
     * TODO Add error handling, e. g. no collision can occur when there is only 1 satellite in the collection!
     * @return Breakup Simulation
     */
    std::unique_ptr<Breakup> getCollision() const;

    /**
     * Creates a new Breakup Simulation with the given input.
     * Can either be a Collision or an Explosion depending on the satellite number in the SatelliteCollection.
     * TODO REMOVE, Determine only by input!
     * @return Breakup Simulation
     */
    std::unique_ptr<Breakup> getBreakupTypeByInput() const;


};
