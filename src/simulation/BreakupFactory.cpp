#include "BreakupFactory.h"

BreakupFactory &BreakupFactory::reconfigure(const std::shared_ptr<ConfigurationSource> &configurationReader) {
    _configurationReader = configurationReader;
    _minimalCharacteristicLength = configurationReader->getMinimalCharacteristicLength();
    _simulationType = configurationReader->getTypeOfSimulation();
    _currentMaximalGivenID = configurationReader->getCurrentMaximalGivenID();
    _idFilter = configurationReader->getIDFilter();
    _satellites = configurationReader->getDataReader()->getSatelliteCollection();
    return *this;
}

std::unique_ptr<Breakup> BreakupFactory::getBreakup() const {
    auto satelliteVector = this->applyFilter();

    switch (_simulationType) {
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
