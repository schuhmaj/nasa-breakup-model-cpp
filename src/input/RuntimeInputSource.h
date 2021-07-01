#pragma once

#include <utility>

#include "input/ConfigurationSource.h"
#include "input/DataSource.h"

/**
 * A object which defines all parameters needed for the Simulation.
 */
class RuntimeInputSource : public ConfigurationSource, public DataSource {

    double _minimalCharacteristicLength;

    SimulationType _simulationType

    size_t _currentMaximalGivenID;

    std::optional<std::set<size_t>> _idFilter;

    std::vector<Satellite> _satellites;

public:

    /**
     * Constructs a new Runtime Source with the minimal requirements needed for the Breakup Builder.
     * @param minimalCharacteristicLength - double
     * @param satellites - satellite vector
     */
    RuntimeInputSource(double minimalCharacteristicLength, std::vector<Satellite> satellites)
            : _minimalCharacteristicLength{minimalCharacteristicLength},
              _simulationType{SimulationType::UNKNOWN},
              _currentMaximalGivenID{0},
              _idFilter{std::nullopt},
              _satellites{std::move(satellites)} {}

    /**
     * Constructs a new Runtime Source with all parameters available.
     * @param minimalCharacteristicLength - double
     * @param simulationType - type of simulation --> strong definition, error handling possible
     * @param currentMaximalGivenId - maximal given NORAD Catalog ID
     * @param idFilter - filter which satellites to use
     * @param satellites - satellite vector
     */
    RuntimeInputSource(double minimalCharacteristicLength, SimulationType simulationType, size_t currentMaximalGivenId,
                       std::optional<std::set<size_t>> idFilter, std::vector<Satellite> satellites)
            : _minimalCharacteristicLength{minimalCharacteristicLength},
              _simulationType{simulationType},
              _currentMaximalGivenID{currentMaximalGivenId},
              _idFilter{std::move(idFilter)},
              _satellites{std::move(satellites)} {}


    double getMinimalCharacteristicLength() override {
        return _minimalCharacteristicLength;
    }

    SimulationType getTypeOfSimulation() override {
        return _simulationType;
    }

    size_t getCurrentMaximalGivenID() override {
        return _currentMaximalGivenID;
    }

    std::shared_ptr<DataSource> getDataReader() override {
        return *this;
    }

    std::optional<std::set<size_t>> getIDSelection() override {
        return _idFilter;
    }

    std::vector<Satellite> getSatelliteCollection() override {
        return _satellites;
    }
};