#include "BreakupBuilder.h"

BreakupBuilder &BreakupBuilder::reconfigure(const std::shared_ptr<ConfigurationSource> &configurationSource) {
    _configurationSource = configurationSource;
    _minimalCharacteristicLength = configurationSource->getMinimalCharacteristicLength();
    _simulationType = configurationSource->getTypeOfSimulation();
    _currentMaximalGivenID = configurationSource->getCurrentMaximalGivenID();
    _idFilter = configurationSource->getIDFilter();
    _satellites = configurationSource->getDataReader()->getSatelliteCollection();
    return *this;
}

BreakupBuilder &BreakupBuilder::reloadConfigurationSource() {
    this->reconfigure(_configurationSource);
    return *this;
}

BreakupBuilder &BreakupBuilder::setMinimalCharacteristicLength(double minimalCharacteristicLength) {
    _minimalCharacteristicLength = minimalCharacteristicLength;
    return *this;
}

BreakupBuilder &BreakupBuilder::setSimulationType(SimulationType simulationType) {
    _simulationType = simulationType;
    return *this;
}

BreakupBuilder &BreakupBuilder::setCurrentMaximalGivenID(size_t currentMaximalGivenID) {
    _currentMaximalGivenID = currentMaximalGivenID;
    return *this;
}

BreakupBuilder &BreakupBuilder::setIDFilter(const std::optional<std::set<size_t>> &idFilter) {
    _idFilter = idFilter;
    return *this;
}

BreakupBuilder &BreakupBuilder::setDataSource(const std::vector<Satellite> &satellites) {
    _satellites = satellites;
    return *this;
}

BreakupBuilder &BreakupBuilder::setDataSource(const std::shared_ptr<DataSource> &dataSource) {
    _satellites = dataSource->getSatelliteCollection();
    return *this;
}

std::unique_ptr<Breakup> BreakupBuilder::getBreakup() const {
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

std::unique_ptr<Breakup> BreakupBuilder::createExplosion(std::vector<Satellite> &satelliteVector) const  {
    return std::make_unique<Explosion>(satelliteVector, _minimalCharacteristicLength, _currentMaximalGivenID);
}

std::unique_ptr<Breakup> BreakupBuilder::createCollision(std::vector<Satellite> &satelliteVector) const {
    return std::make_unique<Collision>(satelliteVector, _minimalCharacteristicLength, _currentMaximalGivenID);
}

std::vector<Satellite> BreakupBuilder::applyFilter() const {
    if (_idFilter.has_value()) {
        std::vector<Satellite> satellitesFiltered{_satellites};
        satellitesFiltered.erase(std::remove_if(satellitesFiltered.begin(), satellitesFiltered.end(),
                                                [&](Satellite &sat) {
                                                    return _idFilter->count(sat.getId()) == 1;
                                                }),
                                 satellitesFiltered.end());
        return satellitesFiltered;
    } else {
        return _satellites;
    }
}
