#include "BreakupFactory.h"

BreakupFactory &BreakupFactory::changeInputSource(const std::shared_ptr<DataReader> &inputReader) {
    _inputReader = inputReader;
    return *this;
}

std::unique_ptr<Breakup> BreakupFactory::getExplosion() const {
    auto satelliteVector{_inputReader->getSatelliteCollection()};
    if (satelliteVector.size() == 1) {
        return std::make_unique<Explosion>(_inputReader->getSatelliteCollection());
    } else {
        throw std::invalid_argument{"The BreakupFactory did not create a new Explosion because there was more than "
                                    "one satellite in the given Input Source!"};
    }
}

std::unique_ptr<Breakup> BreakupFactory::getCollision() const {
    auto satelliteVector{_inputReader->getSatelliteCollection()};
    if (satelliteVector.size() == 2) {
        return std::make_unique<Collision>(_inputReader->getSatelliteCollection());
    } else {
        throw std::invalid_argument{"The BreakupFactory did not create a new Explosion because there was not the right "
                                    "number of satellites in the given input Source (exactly 2 satellites are required!"};
    }
}

std::unique_ptr<Breakup> BreakupFactory::getBreakupTypeByInput() const {
    auto satelliteVector{_inputReader->getSatelliteCollection()};
    if (satelliteVector.size() == 1) {
        return std::make_unique<Explosion>(satelliteVector);
    } else if (satelliteVector.size() == 2 ) {
        return std::make_unique<Collision>(satelliteVector);
    } else {
        throw std::invalid_argument{"The BreakupFactory did not create a new Explosion which requires 1 satellites or "
                                    "an Collision which requires 2 satellites. So the input source contained either 0 "
                                    "or more than two satellites!"};
    }
}
