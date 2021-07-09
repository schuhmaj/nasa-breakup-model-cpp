#pragma once

#include <utility>
#include <tuple>
#include "model/OrbitalElements.h"
#include "util/UtilityKepler.h"
#include "util/UtilityFunctions.h"

/**
 * Provides methods to create Orbital Elements.
 */
class OrbitalElementsFactory {

public:

    OrbitalElementsFactory() = default;

    /**
    * Calculates the cartesian velocity and cartesian position of this satellite by using the Keplerian Elements.
    * This function sets the internal members _velocity and _position to the corresponding values.
    * @param keplerianElements array holds the arguments in the following order:<br>
    * mm - mean motion [revolutions/day]<br>
    * e - eccentricity<br>
    * i - inclination [deg]<br>
    * W - longitude of the ascending node (big omega) [deg]<br>
    * w - argument of periapsis (small omega) [deg]<br>
    * MA - Mean Anomaly [deg]<br>
    */
    OrbitalElements fromTLEData(const std::array<double, 6> &tleData) const;

    /**
    * Constructs the Orbital Elements from the standard keplerian Elements in the following order:
    * @param keplerianElements array holds the arguments in the following order:<br>
    * a - semir-major axis [m]<br>
    * e - eccentricity<br>
    * i - inclination [rad]<br>
    * W - longitude of the ascending node (big omega) [rad]<br>
    * w - argument of periapsis (small omega) [rad]<br>
    * A - an anomaly in [rad]<br>
    * @param orbitalAnomalyType - this defines the last element in the array, either: ECCENTRIC, MEAN or TRUE
    */
    OrbitalElements
    fromOnlyRadians(const std::array<double, 6> &standardKepler, OrbitalAnomalyType orbitalAnomalyType) const;

    /**
    * Constructs the Orbital Elements from the standard keplerian Elements in the following order:
    * @param keplerianElements array holds the arguments in the following order:<br>
    * a - semir-major axis [m]<br>
    * e - eccentricity<br>
    * i - inclination [deg]<br>
    * W - longitude of the ascending node (big omega) [deg]<br>
    * w - argument of periapsis (small omega) [deg]<br>
    * A - an anomaly in [deg]<br>
    * @param orbitalAnomalyType - this defines the last element in the array, either: ECCENTRIC, MEAN or TRUE
    */
    OrbitalElements
    fromOnlyDegree(const std::array<double, 6> &standardKepler, OrbitalAnomalyType orbitalAnomalyType) const;


};
