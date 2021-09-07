#pragma once

#include <cmath>

namespace util {

    /**
     * PI with enough precision for future computers :)
     * (and as constexpr with no preprocessor magic)
     */
    constexpr double PI = 3.1415926535897932384626433832795028841971693993751058209749445923;

    /**
    * PI multiplied by 2 with enough precision
    * (and as constexpr with no preprocessor magic)
    */
    constexpr double PI2 = 6.2831853071795864769252867665590057683943387987502116419498891846;

    /**
     * PI divided by 4 with enough precision
     * (and as constexpr with no preprocessor magic)
     */
    constexpr double PI_4 = 0.7853981633974483096156608458198757210492923498437764552437361480;

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
        static constexpr double MultPI4_3 = 4.0 / 3.0 * PI;

        double radius = characteristicLength / 2.0;
        return MultPI4_3 * (radius * radius * radius) * calculateDensity(characteristicLength);
    }

    /**
    * Calculates the Characteristic Length assuming the mass is formed like a sphere.
    * @param mass in [kg] of the satellite
    * @return diameter/ characteristic Length in [m]
    */
    inline double calculateCharacteristicLengthFromMass(double mass) {
        static constexpr double Mul92_937PI = 92.937 * PI;
        static constexpr double Inv2_26 = 1.0 / 2.26;
        return std::pow((6.0 * mass) / (Mul92_937PI), Inv2_26);
    }

    /**
     * Calculates the Area to Mass Ratio (A/M) based on BSTAR from TLE Data.
     * @param bstar
     * @return A/M in [m^2/kg]
     */
    inline double calculateAreaToMassRatio(double bstar, double a, double e) {
        static constexpr double C_d = 2.2;
        static constexpr double p_0 = 0.1570; //0.00002461;           //2.461 * 10^(-5)
        static constexpr double factor = 2.0 / p_0 / C_d;
        double am = bstar * factor;
        double periapsis = (1 - e) * a;
        //Scaling:

        //Iridium 33
//        if (periapsis < 750000) {
//            return 79.44 * am;
//        } else {
//            return 74.42 * am;
//        }

        //Kosmos 2251
        if (periapsis < 500000) {
            return 21.56 * am;
        } else if (periapsis <= 500000 && periapsis <= 700000) {
            return 67.77 * am;
        } else {
            return 80.05 * am;
        }

//        //Fengyun 1-C
//        if (periapsis < 600000) {
//            return 28.85 * am;
//        } else if (periapsis <= 600000 && periapsis <= 800000) {
//            return 76.48 * am;
//        } else {
//            return 73.09 * am;
//        }
    }

    /**
     * Transform an y in [0;1] generated by an uniform distribution to an x which follows the
     * properties of a power law distribution.
     * @param x0 - the lower bound for the numbers (corresponds to the minimal L_c)
     * @param x1 - the upper bound (correspond to the maximum of L_c of the two satellites or infinite if
     *              there is no upper bound)
     * @param n - the exponent from the power law distribution, more precisely the exponent of the
     *              probability density function (pdf)
     * @param y - the value from the uniform distribution to transform
     * @return the transformed x following the power law distribution
     */
    inline double transformUniformToPowerLaw(double x0, double x1, double n, double y) {
        double step = (std::pow(x1, n + 1.0) - std::pow(x0, n + 1.0)) * y + std::pow(x0, n + 1.0);
        return std::pow(step, 1.0 / (n + 1.0));
    }

    /**
     * Converts an angle [deg] to [rad]
     * @param angle in [deg]
     * @return angle in [rad]
     */
    inline double degToRad(double angle) {
        static constexpr double DivPI180 = PI / 180.0;
        return angle * DivPI180;
    }

    /**
     * Converts an angle [rad] to [deg]
     * @param angle in [rad]
     * @return angle in [deg]
     */
    inline double radToDeg(double angle) {
        static constexpr double Div180PI = 180.0 / PI;
        return angle * Div180PI;
    }

}