#pragma once

#include <utility>

#include "input/InputConfigurationSource.h"
#include "input/DataSource.h"

/**
 * A object which defines all parameters needed for the Simulation.
 */
class RuntimeInputSource : public InputConfigurationSource, public DataSource, public std::enable_shared_from_this<const DataSource> {

    const double _minimalCharacteristicLength;

    const SimulationType _simulationType;

    const size_t _currentMaximalGivenID;

    const std::optional<std::set<size_t>> _idFilter;

    const std::vector<Satellite> _satellites;

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
     * Constructs a new Runtime Source with four parameters available (all expect Filter)
     * @param minimalCharacteristicLength - double
     * @param simulationType - type of simulation --> strong definition, error handling possible
     * @param currentMaximalGivenId - maximal given NORAD Catalog ID
     * @param satellites - satellite vector
     */
    RuntimeInputSource(double minimalCharacteristicLength, SimulationType simulationType, size_t currentMaximalGivenId,
                       std::vector<Satellite> satellites)
            : _minimalCharacteristicLength{minimalCharacteristicLength},
              _simulationType{simulationType},
              _currentMaximalGivenID{currentMaximalGivenId},
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

    /**
     * Constructs a new Runtime Source with all parameters available.
     * Here you give the RuntimeInputSource another DataSource like an TLESatcatReader or a YAMLDataReader to configure
     * settings via this object but with advantage of file input.
     * @param minimalCharacteristicLength - double
     * @param simulationType - type of simulation --> strong definition, error handling possible
     * @param currentMaximalGivenId - maximal given NORAD Catalog ID
     * @param idFilter - filter which satellites to use
     * @param dataSource - a DataSource like TLESatcatReader or YAMLDataReader
     */
    RuntimeInputSource(double minimalCharacteristicLength, SimulationType simulationType, size_t currentMaximalGivenId,
                       std::optional<std::set<size_t>> idFilter, const std::shared_ptr<DataSource>& dataSource)
            : _minimalCharacteristicLength{minimalCharacteristicLength},
              _simulationType{simulationType},
              _currentMaximalGivenID{currentMaximalGivenId},
              _idFilter{std::move(idFilter)},
              _satellites{dataSource->getSatelliteCollection()} {}


    double getMinimalCharacteristicLength() const override;

    SimulationType getTypeOfSimulation() const override;

    size_t getCurrentMaximalGivenID() const override;

    std::shared_ptr<const DataSource> getDataReader() const override;

    std::optional<std::set<size_t>> getIDFilter() const override;

    std::vector<Satellite> getSatelliteCollection() const override;
};