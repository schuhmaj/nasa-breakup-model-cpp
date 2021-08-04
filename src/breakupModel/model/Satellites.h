#pragma once

#include <vector>
#include <array>
#include <variant>
#include <tuple>
#include <algorithm>
#include <memory>

#include "Satellite.h"

/**
 * This class implements the Satellites in an SoA (Structure of Array) way.
 * This is especially useful for the BreakupSimulation to vectorized calculation and save memory for shared
 * properties of the fragment satellites created.
 * @note This class provides a method to transform the SoA structure into an AoS approach (std::vector<Satellite>)
 */
class Satellites {

    /*
     * Shared Properties
     */

    /**
     * The NORAD Catalog ID of the first Satellite in the SoA
     */
    size_t _startID;

    /**
     * The SatType of the Satellites in the SoA
     */
    SatType _satType;

    /**
     * The position base of the Satellites in the SoA
     * This is a cartesian position vector in [m]
     */
    std::array<double, 3> _position;

    /*
     * Unique Properties
     */

    /**
     * The name of each of the Satellites in the SoA
     */
    std::vector<std::shared_ptr<const std::string>> _name;

    /**
     * The characteristic length of each satellite in [m]
     */
    std::vector<double> _characteristicLength;

    /**
     * The area-to-mass ratio of each satellite in [m^2/kg]
     */
    std::vector<double> _areaToMassRatio;

    /**
     * The mass of each satellite in [kg]
     */
    std::vector<double> _mass;

    /**
     * The area/ Radar-Cross-Section of each satellite in [m^2]
     */
    std::vector<double> _area;

    /**
     * The ejection velocity of each satellite in [m/s]
     * This is a cartesian velocity vector.
     */
    std::vector<std::array<double, 3>> _ejectionVelocity;

    /**
     * The velocity of each satellite in [m/s]
     * This is a cartesian velocity vector in which each element is the sum of ejection and (parental) base velocity
     */
    std::vector<std::array<double, 3>> _velocity;

    Satellites(size_t startID, SatType satType, std::array<double, 3> position, size_t reserveMemory)
            : _startID{startID},
              _satType{satType},
              _position{position} {
        _name.reserve(reserveMemory);
        _characteristicLength.reserve(reserveMemory);
        _areaToMassRatio.reserve(reserveMemory);
        _mass.reserve(reserveMemory);
        _area.reserve(reserveMemory);
        _ejectionVelocity.reserve(reserveMemory);
        _velocity.reserve(reserveMemory);
    }

    Satellites(size_t startID, SatType satType, std::array<double, 3> position, const std::string &name1,
               const std::string &name2, size_t reserveMemory)
            : _startID{startID},
              _satType{satType},
              _position{position} {
        _name.reserve(reserveMemory);
        _characteristicLength.reserve(reserveMemory);
        _areaToMassRatio.reserve(reserveMemory);
        _mass.reserve(reserveMemory);
        _area.reserve(reserveMemory);
        _ejectionVelocity.reserve(reserveMemory);
        _velocity.reserve(reserveMemory);
    }

    /**
     * Returns a manipulatable tuple view for the unique attributes of the Satellites.
     * @return vector of tuples [namePtr, L_c, A/M, m, a, V_ejection, V]
     * @note Especially useful for the advanced for loop
     */
    std::vector<std::tuple<std::shared_ptr<const std::string> &, double &, double &,
    double &, double &, std::array<double, 3> &, std::array<double, 3> &>> getAsTuple();

    /**
     * Returns this Structure of Arrays as an Array of Structures.
     * @return vector of Satellites
     */
    std::vector<Satellite> getAoS();

};
