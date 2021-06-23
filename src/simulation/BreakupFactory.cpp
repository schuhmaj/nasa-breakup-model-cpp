#include "BreakupFactory.h"

BreakupFactory &BreakupFactory::changeConfiguration(const std::shared_ptr<ConfigurationReader> &configurationReader) {
    _configurationReader = configurationReader;
    return *this;
}

std::unique_ptr<Breakup> BreakupFactory::getBreakup() const {
    auto satelliteVector{_dataReader->getSatelliteCollection()};

    switch (_configurationReader->getTypeOfSimulation()) {
        case SimulationType::EXPLOSION:
            if (satelliteVector.size() == 1) {
                return createExplosion(satelliteVector);
            }
        case SimulationType::COLLISION:
            if (satelliteVector.size() == 2) {
                return createCollision(satelliteVector);
            }
        default:
            if (satelliteVector.size() == 1) {
                std::cerr << "Type was not specified by configuration file, Derived Explosion from 1 satellite!";
                return createExplosion(satelliteVector);
            } else if (satelliteVector.size() == 2) {
                std::cerr << "Type was not specified by configuration file, Derived Collision from 2 satellites!";
                return createCollision(satelliteVector);
            } else {
                throw std::invalid_argument{"A breakup simulation could not be created because the type given"
                                            "by the configuration file was different than the number of"
                                            "satellites in the given data input would suggest. Notice:\n"
                                            "Explosion --> 1 satellite\n"
                                            "Collision --> 2 satellites"};
            }
    }
}
