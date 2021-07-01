#pragma once

#include <memory>
#include <exception>
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
     * A explosion needs 1 satellite! Not more not less!
     * @return Breakup Simulation
     * @throws an invalid_argument exception if not exactly one satellite is in the input source
     */
    std::unique_ptr<Breakup> getExplosion() const;

    /**
     * Creates a new Collision Breakup Simulation with the given input.
     * A collision needs 2 satellites! Not more not less!
     * @return Breakup Simulation
     * @throws an invalid_argument exception if not exactly two satellites are in the input source
     */
    std::unique_ptr<Breakup> getCollision() const;

    /**
     * Creates a new Breakup Simulation with the given input.
     * Can either be a Collision or an Explosion depending on the satellite number in the SatelliteCollection.
     * TODO REMOVE, Determine only by input!
     * @return Breakup Simulation
     * @throws an invalid_argument exception if zero or more than two satellites are in the input source
     */
    std::unique_ptr<Breakup> getBreakupTypeByInput() const;


};
