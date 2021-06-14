#include "BreakupFactory.h"

BreakupFactory &BreakupFactory::changeInputSource(const std::shared_ptr<InputReader> &inputReader) {
    _inputReader = inputReader;
    return *this;
}

std::unique_ptr<Breakup> BreakupFactory::getExplosion() const {
    return std::make_unique<Explosion>(_inputReader->getSatelliteCollection());
}

std::unique_ptr<Breakup> BreakupFactory::getCollision() const {
    return std::make_unique<Collision>(_inputReader->getSatelliteCollection());
}

std::unique_ptr<Breakup> BreakupFactory::getBreakupTypeByInput() const {
    SatelliteCollection satelliteCollection{_inputReader->getSatelliteCollection()};
    if (satelliteCollection.size() == 1) {
        return std::make_unique<Explosion>(satelliteCollection);
    } else {
        return std::make_unique<Collision>(satelliteCollection);
    }
}
