#pragma once

#include <cmath>

/**
 * PI with enough precision for future computers :)
 * (and as constexpr and no preprocessor magic)
 */
constexpr double PI = 3.1415926535897932384626433832795028841971693993751058209749445923;
/**
 * Density of Aluminium in [kg/m^3]
 */
constexpr double ALUMINIUM_DENSITY = 2698.9;

/**
 * Calculates the density according to Equation 1.
 * If the density is smaller than 1cm, the density of Aluminium is returned.
 * @param characteristicLength in [m] which correspond to the diameter
 * @return density in [kg/m^3]
 */
inline double calculateDensity(double characteristicLength) {
    return characteristicLength < 0.01 ? ALUMINIUM_DENSITY : 92.937 * std::pow(characteristicLength, -0.74);
}

/**
 * Calculates the circle area for a given characteristic length.
 * @param characteristicLength in [m] which correspond to the diameter
 * @return area in [m^2]
 */
inline double calculateCircleArea(double characteristicLength) {
    double radius = characteristicLength / 2.0;
    return PI * radius * radius;
}

/**
 * Calculates the characteristicLength (= diameter) for a given circle area.
 * @param area in [m^2]
 * @return diameter in [m]
 */
inline double calculateCharacteristicLength(double area) {
    return 2.0 * sqrt(area / PI);
}

/**
 * Calculates the mass according to a characteristicLength assuming the object is a sphere.
 * @param characteristicLength in [m] which correspond to the diameter
 * @return mass in [kg]
 */
inline double calculateSphereMass(double characteristicLength) {
    double radius = characteristicLength / 2.0;
    return 4.0 / 3.0 * PI * (radius * radius * radius) * calculateDensity(characteristicLength);
}

inline double calculateCharacteristicLengthFromMass(double mass) {
    return std::pow( (6 * mass) / (92.937 * PI), 1 / 2.26 );
}
