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
    double _semiMajorAxis{0};

    /**
     * The eccentricity (unit-less)
     */
    double _eccentricity{0};

    /**
     * The inclination in [rad]
     */
    double _inclination{0};

    /**
     * The longitude-of-the-ascending-node or also called RAAN in [rad]
     */
    double _longitudeOfTheAscendingNode{0};

    /**
     * The argument-of-periapsis in [rad]
     */
    double _argumentOfPeriapsis{0};

    /**
     * One orbital anomaly given in [rad]
     */
    double _eccentricAnomaly{0};

public:

    OrbitalElements() = default;

    /**
    * Creates the Orbital Elements.
    * @param a - semir-major axis [m]
    * @param e - eccentricity
    * @param i - inclination [rad]
    * @param W - longitude of the ascending node (big omega) [rad]
    * @param w - argument of periapsis (small omega) [rad]
    * @param EA - eccentric Anomaly [rad]
    * This is the "inverse" of getAsArray.
    * @note Use the OrbitalElementsFactory instead of this constructor!
    */
    OrbitalElements(double semiMajorAxis, double eccentricity, double inclination, double longitudeOfTheAscendingNode,
                    double argumentOfPeriapsis, double eccentricAnomaly) :
            _semiMajorAxis(semiMajorAxis),
            _eccentricity(eccentricity),
            _inclination(inclination),
            _longitudeOfTheAscendingNode(longitudeOfTheAscendingNode),
            _argumentOfPeriapsis(argumentOfPeriapsis),
            _eccentricAnomaly(eccentricAnomaly) {}

    /**
     * Creates the Orbital Elements from an array which contains the six elements in the following order:
     * @param uniformOrbitalElements<br>
     * a - semir-major axis [m]<br>
     * e - eccentricity<br>
     * i - inclination [rad]<br>
     * W - longitude of the ascending node (big omega) [rad]<br>
     * w - argument of periapsis (small omega) [rad]<br>
     * EA - eccentric Anomaly [rad]<br>
     * This is the "inverse" of getAsArray.
     * @note Use the OrbitalElementsFactory instead of this constructor!
     */
    explicit OrbitalElements(const std::array<double, 6> &uniformOrbitalElements)
            : _semiMajorAxis{uniformOrbitalElements[0]},
              _eccentricity{uniformOrbitalElements[1]},
              _inclination{uniformOrbitalElements[2]},
              _longitudeOfTheAscendingNode{uniformOrbitalElements[3]},
              _argumentOfPeriapsis{uniformOrbitalElements[4]},
              _eccentricAnomaly{uniformOrbitalElements[5]} {}

    /**
     * Returns the orbital Element in an array. The semi-major-axis in [m], the eccentricity unit-less,
     * the angles in [RAD] and lastly the eccentric anomaly in [RAD] too.
     * This is the "inverse" of the OrbitalElements(const std::array<double, 6>) constructor.
     * @return array<a, e, i, W, w, eccentric-anomaly>
     */
    std::array<double, 6> getAsArray() const;

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
     * Returns the inclination in [DEG] or [RAD] (default)
     * @param angularUnit
     * @return inclination
     */
    double getInclination(AngularUnit angularUnit = AngularUnit::RADIAN) const;

    /**
    * Returns the longitude-of-the-ascending-node in [DEG] or [RAD] (default)
    * @param angularUnit
    * @return longitude-of-the-ascending-node
    */
    double getLongitudeOfTheAscendingNode(AngularUnit angularUnit = AngularUnit::RADIAN) const;

    /**
    * Returns the argument-of-periapsis in [DEG] or [RAD] (default)
    * @param angularUnit
    * @return argument-of-periapsis
    */
    double getArgumentOfPeriapsis(AngularUnit angularUnit = AngularUnit::RADIAN) const;

    /**
    * Returns an orbital anomaly in [DEG] or [RAD] (default); Available types are MEAN/ ECCENTRIC (default) / TRUE Anomaly.
    * @param angularUnit
    * @return orbital anomaly
    */
    double getAnomaly(AngularUnit angularUnit = AngularUnit::RADIAN,
                      OrbitalAnomalyType orbitalAnomalyType = OrbitalAnomalyType::ECCENTRIC) const;

    /**
     * Compares two OrbitalElements for equality.
     * @param lhs - OrbitalElements
     * @param rhs - OrbitalElements
     * @return true if they are exactly the same
     */
    friend bool operator==(const OrbitalElements &lhs, const OrbitalElements &rhs);

    /**
     * Compares two OrbitalElements for inequality.
     * @param lhs - OrbitalElements
     * @param rhs - OrbitalElements
     * @return true if they differ
     */
    friend bool operator!=(const OrbitalElements &lhs, const OrbitalElements &rhs);

private:

    /**
     * Converts a given angle into a given target unit
     * @param angle - angle in [rad]
     * @param targetAngularUnit - target unit
     * @return the value in the target unit
     */
    static double convertAngle(double angle, AngularUnit targetAngularUnit);

    /**
     * Converts a given eccentric anomaly into a given target anomaly in a specific target angular unit.
     * @param eccentricAnomaly - eccentric anomaly in [rad]
     * @param eccentricity - the eccentricity
     * @param targetAngularUnit - the target angular unit
     * @param targetOrbitalAnomalyType - target anomaly MEAN/ ECCENTRIC/ TRUE Anomaly
     * @return value of chosen anomaly in target angular unit
     */
    static double convertEccentricAnomaly(double eccentricAnomaly, double eccentricity, AngularUnit targetAngularUnit,
                                          OrbitalAnomalyType targetOrbitalAnomalyType);

};