#include "BreakupFactory.h"

BreakupFactory &BreakupFactory::changeInputSource(const std::shared_ptr<InputReader> &inputReader) {
    _inputReader = inputReader;
    return *this;
}

Breakup BreakupFactory::getExplosion() const {
    return Explosion{_inputReader->getSatelliteCollection()};
}

Breakup BreakupFactory::getCollision() const {
    return Collision{_inputReader->getSatelliteCollection()};
}
