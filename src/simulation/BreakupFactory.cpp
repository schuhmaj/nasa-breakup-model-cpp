#include "BreakupFactory.h"

BreakupFactory &BreakupFactory::reconfigure(const std::shared_ptr<ConfigurationSource> &configurationSource) {
    _configurationSource = configurationSource;
    _minimalCharacteristicLength = configurationSource->getMinimalCharacteristicLength();
    _simulationType = configurationSource->getTypeOfSimulation();
    _currentMaximalGivenID = configurationSource->getCurrentMaximalGivenID();
    _idFilter = configurationSource->getIDFilter();
    _satellites = configurationSource->getDataReader()->getSatelliteCollection();
    return *this;
}

BreakupFactory &BreakupFactory::reloadConfigurationSource() {
    this->reconfigure(_configurationSource);
    return *this;
}

BreakupFactory &BreakupFactory::setMinimalCharacteristicLength(double minimalCharacteristicLength) {
    _minimalCharacteristicLength = minimalCharacteristicLength;
    return *this;
}

BreakupFactory &BreakupFactory::setSimulationType(SimulationType simulationType) {
    _simulationType = simulationType;
    return *this;
}

BreakupFactory &BreakupFactory::setCurrentMaximalGivenID(size_t currentMaximalGivenID) {
    _currentMaximalGivenID = currentMaximalGivenID;
    return *this;
}

BreakupFactory &BreakupFactory::setIDFilter(const std::optional<std::set<size_t>> &idFilter) {
    _idFilter = idFilter;
    return *this;
}

BreakupFactory &BreakupFactory::setDataSource(const std::vector<Satellite> &satellites) {
    _satellites = satellites;
    return *this;
}

BreakupFactory &BreakupFactory::setDataSource(const std::shared_ptr<DataSource> &dataSource) {
    _satellites = dataSource->getSatelliteCollection();
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
