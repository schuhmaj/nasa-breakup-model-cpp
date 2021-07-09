#pragma once

#include <utility>
#include <tuple>
#include <array>
#include <exception>
#include "util/UtilityKepler.h"
#include "util/UtilityFunctions.h"

/**
 * Unit of Angles
 */
enum class AngularUnit {
    DEGREE, RADIAN
};

/**
 * Three different Orbital Anomalies are possible
 */
enum class OrbitalAnomalyType {
    ECCENTRIC, MEAN, TRUE
};


class OrbitalElements {

    /**
     * The semi-major-axis in [m]
     */
    const double _semiMajorAxis;

    /**
     * The eccentricity (unit-less)
     */
    const double _eccentricity;

    /**
     * The inclination in either [deg] or [rad]
     */
    const std::pair<double, AngularUnit> _inclination;

    /**
     * The longitude-of-the-ascending-node or also called RAAN in [deg] or [rad]
     */
    const std::pair<double, AngularUnit> _longitudeOfTheAscendingNode;

    /**
     * The argument-of-periapsis in [deg] or [rad]
     */
    const std::pair<double, AngularUnit> _argumentOfPeriapsis;

    /**
     * One orbital anomaly given in [deg] or [rad]
     */
    const std::tuple<double, AngularUnit, OrbitalAnomalyType> _anomaly;

public:

    OrbitalElements(double semiMajorAxis, double eccentricity,
                    std::pair<double, AngularUnit> inclination,
                    std::pair<double, AngularUnit> longitudeOfTheAscendingNode,
                    std::pair<double, AngularUnit> argumentOfPeriapsis,
                    std::tuple<double, AngularUnit, OrbitalAnomalyType> anomaly)
            : _semiMajorAxis{semiMajorAxis},
              _eccentricity{eccentricity},
              _inclination{std::move(inclination)},
              _longitudeOfTheAscendingNode{std::move(longitudeOfTheAscendingNode)},
              _argumentOfPeriapsis{std::move(argumentOfPeriapsis)},
              _anomaly{std::move(anomaly)} {}

    /**
    * Creates the Orbital Elements from the TLE Data set which contains the following elements
    * @param meanMotion in [rev/day]
    * @param eccentricity (unit-less)
    * @param inclination in [deg]
    * @param longitudeOfTheAscendingNode in [deg]
    * @param argumentOfPeriapsis in [deg]
    * @param meanAnomaly in [deg]
    */
    explicit OrbitalElements(double meanMotion, double eccentricity, double inclination,
                             double longitudeOfTheAscendingNode, double argumentOfPeriapsis, double meanAnomaly)
            : _semiMajorAxis{util::meanMotionToSemiMajorAxis(meanMotion)},
              _eccentricity{eccentricity},
              _inclination{std::make_pair(inclination, AngularUnit::DEGREE)},
              _longitudeOfTheAscendingNode{std::make_pair(longitudeOfTheAscendingNode, AngularUnit::DEGREE)},
              _argumentOfPeriapsis{std::make_pair(argumentOfPeriapsis, AngularUnit::DEGREE)},
              _anomaly{std::make_tuple(meanAnomaly, AngularUnit::DEGREE, OrbitalAnomalyType::MEAN)} {}


    /**
     * Creates the Orbital Elements from the Uniform array which contains the six elements in the from:
     * The semi-major-axis in [m], the eccentricity unit-less,
     * the angles in [RAD] and lastly the eccentric anomaly in [RAD] too.
     * This is the "inverse" of getAsUniform.
     * @param uniformOrbitalElements
     */
    explicit OrbitalElements(const std::array<double, 6> &uniformOrbitalElements)
            : _semiMajorAxis{uniformOrbitalElements[0]},
              _eccentricity{uniformOrbitalElements[1]},
              _inclination{std::make_pair(uniformOrbitalElements[2], AngularUnit::RADIAN)},
              _longitudeOfTheAscendingNode{std::make_pair(uniformOrbitalElements[3], AngularUnit::RADIAN)},
              _argumentOfPeriapsis{std::make_pair(uniformOrbitalElements[4], AngularUnit::RADIAN)},
              _anomaly{std::make_tuple(uniformOrbitalElements[5], AngularUnit::RADIAN, OrbitalAnomalyType::ECCENTRIC)} {}

    /**
     * Returns the orbital Element in an array. The semi-major-axis in [m], the eccentricity unit-less,
     * the angles in [RAD] and lastly the eccentric anomaly in [RAD] too.
     * This is the "inverse" of the OrbitalElements(const std::array<double, 6>) constructor.
     * @return array<a, e, i, W, w, eccentric-anomaly>
     */
    std::array<double, 6> getAsUniform() const;

    /**
     * Returns one element from the uniform "view" of the Keplerian Elements.
     * @param index in the array
     * @return element in array<a, e, i, W, w, eccentric-anomaly>
     * @throws an out_of_range exception if index out of range [0;5]
     */
    double operator[](size_t index) const;

    /**
     * Returns the semi-major-axis in [m]
     * @return semi-major-axis
     */
    double getSemiMajorAxis() const;

    /**
     * Returns the eccentricity (unit-less)
     * @return eccentricity
     */
    double getEccentricity() const;

    /**
     * Returns the inclination in [DEG] or [RAD]
     * @param angularUnit
     * @return inclination
     */
    double getInclination(AngularUnit angularUnit) const;

    /**
    * Returns the longitude-of-the-ascending-node in [DEG] or [RAD]
    * @param angularUnit
    * @return longitude-of-the-ascending-node
    */
    double getLongitudeOfTheAscendingNode(AngularUnit angularUnit) const;

    /**
    * Returns the argument-of-periapsis in [DEG] or [RAD]
    * @param angularUnit
    * @return argument-of-periapsis
    */
    double getArgumentOfPeriapsis(AngularUnit angularUnit) const;

    /**
    * Returns an orbital anomaly in [DEG] or [RAD]; Available types are MEAN/ ECCENTRIC/ TRUE Anomaly.
    * @param angularUnit
    * @return orbital anomaly
    */
    double getAnomaly(AngularUnit angularUnit, OrbitalAnomalyType orbitalAnomalyType) const;

private:

    /**
     * Converts a given angle into a given target unit
     * @param angle - pair of value and unit
     * @param targetAngularUnit - target unit
     * @return the value in the target unit
     */
    double convertAngle(const std::pair<double, AngularUnit> &angle, AngularUnit targetAngularUnit) const;

    /**
     * Converts a given orbital anomaly into a given target anomaly in a specific target angular unit.
     * @param anomaly - a tuple of value, unit and anomaly type
     * @param targetAngularUnit - target angular unit
     * @param targetOrbitalAnomalyType - target anomaly MEAN/ ECCENTRIC/ TRUE Anomaly
     * @return value of chosen anomaly in target angular unit
     */
    double convertAnomaly(const std::tuple<double, AngularUnit, OrbitalAnomalyType> &anomaly,
                          AngularUnit targetAngularUnit,
                          OrbitalAnomalyType targetOrbitalAnomalyType) const;

};