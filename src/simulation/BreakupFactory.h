#pragma once

#include <memory>
#include <exception>
#include <iostream>
#include "input/ConfigurationSource.h"
#include "input/DataSource.h"
#include "Breakup.h"
#include "Explosion.h"
#include "Collision.h"

/**
 * Interface to easily create a Breakup Simulation.
 */
class BreakupFactory {

    std::shared_ptr<ConfigurationSource> _configurationReader;

    std::shared_ptr<DataSource> _dataReader;

public:

    explicit BreakupFactory(std::shared_ptr<ConfigurationSource> &configurationReader)
            : _configurationReader{configurationReader},
              _dataReader{configurationReader->getDataReader()} {}

    /**
     * Adds an input source for the satellites.
     * @param configurationReader - a shared pointer to an input source
     * @return this
     */
    BreakupFactory &changeConfiguration(const std::shared_ptr<ConfigurationSource> &configurationReader);

    /**
     * Creates a new Breakup Simulation with the given input.
     * Can either be a Collision or an Explosion depending on the satellite number in the SatelliteCollection.
     * Three types of Input Specification are imaginable:<br>
     * STRONG --> Specified type in config file & Satellite number are harmonic<br>
     * WEAK   --> No specified input type, but Satellite number suggests a type (error message, but simulation continues)<br>
     * NONE   --> No input type given, type cannot be derived from satellite number (throws exception)<br>
     * @return Breakup Simulation
     * @throws an invalid_argument exception if type is not determined
     */
    std::unique_ptr<Breakup> getBreakup() const;


private:

    /**
     * Creates a Explosion Simulation.
     * @param satelliteVector - std::vector<Satellite>
     * @return a Explosion
     */
    inline std::unique_ptr<Breakup> createExplosion(std::vector<Satellite> &satelliteVector) const {
        return std::make_unique<Explosion>(satelliteVector, _configurationReader->getMinimalCharacteristicLength(),
                                           _configurationReader->getCurrentMaximalGivenID());
    }

    /**
     * Creates a Collision Simulation.
     * @param satelliteVector - std::vector<Satellite>
     * @return a Collision
     */
    inline std::unique_ptr<Breakup> createCollision(std::vector<Satellite> &satelliteVector) const {
        return std::make_unique<Collision>(satelliteVector, _configurationReader->getMinimalCharacteristicLength(),
                                           _configurationReader->getCurrentMaximalGivenID());
    }
};
