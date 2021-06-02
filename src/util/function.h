#pragma once

#include <cmath>

/**
 * PI with enough precision for future computers :)
 * (and as constexpr and no preprocessor magic)
 */
constexpr double PI = 3.1415926535897932384626433832795028841971693993751058209749445923;

/**
 * Calculates the density according to Equation 1.
 * @param radius/ the characteristic length in [m]
 * @return density in [kg/m^3]
 */
inline double calculateDensity(double radius) {
    return 92.937 * pow(radius*2.0, -0.74);
}

/**
 * Calculates the circle area for a given radius.
 * @param radius or the characteristic length in [m]
 * @return area in [m^2]
 */
inline double calculateCircleArea(double radius) {
    return PI * radius * radius;
}

/**
 * Calculates the radius for a given circle area.
 * @param area in [m^2]
 * @return radius in [m]
 */
inline double calculateCircleRadius(double area) {
    return sqrt(area/PI);
}

/**
 * Calculates the mass according to a radius assuming the object is a sphere.
 * @param radius in [m]
 * @return mass in [kg]
 */
inline double calculateSphereMass(double radius) {
    return 4.0 / 3.0 * PI * (radius * radius * radius) * calculateDensity(radius);
}
