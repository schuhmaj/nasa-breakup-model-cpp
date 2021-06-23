#pragma once

#include <cmath>

/*
 * Functions adapted from pykep:
 * -https://github.com/esa/pykep/blob/master/include/keplerian_toolbox/core_functions/convert_anomalies.hpp
 * -https://github.com/esa/pykep/blob/master/include/keplerian_toolbox/numerics/newton_raphson.hpp
 * -https://github.com/esa/pykep/blob/master/include/keplerian_toolbox/core_functions/kepler_equations.hpp
 * [Links accessed 23.06.2021]
 */

namespace util {

    /**
    * Gravitational Parameter of the earth in [m^3/s^2]
    */
    constexpr double GRAVITATIONAL_PARAMETER_EARTH = 398600441880000.0;

    namespace detail {

        constexpr double accuracy = 1e-16;


        // With the eccentric anomaly (EA)
        inline double kepE(double EA, double MA, double eccentricity) {
            return (EA - eccentricity * sin(EA) - MA);
        }

        inline double d_kepE(double EA, double eccentricity) {
            return (1 - eccentricity * cos(EA));
        }

        inline double newton_raphson(double EA, double MA, double e) {
            int max_loop = 100;
            double term;
            // double start = x; //DS: unused
            // main iteration
            do {
                term = kepE(EA, MA, e) / d_kepE(EA, e);
                EA = EA - term;
            }
                // check if term is within required accuracy or loop limit is exceeded
            while ((fabs(term / std::max(std::fabs(EA), 1.)) > accuracy) && (--max_loop));
            return EA;
        }

    }

    /**
     * Converts the Mean Anomaly to the Eccentric Anomaly.
     * @param MA - Mean Anomaly [rad]
     * @param e - eccentricity
     * @return Eccentric Anomaly in [rad]
     */
    inline double meanAnomalyToEccentricAnomaly(double MA, double e) {
        using namespace detail;
        double EA = MA + e * sin(MA);
        return newton_raphson(EA, MA, e);
    }

    /**
     * Converts the Eccentric Anomaly to the Mean Anomaly.
     * @param EA - Eccentric Anomaly [rad]
     * @param e - eccentricity
     * @return Mean Anomaly in [rad]
     */
    inline double eccentricAnomalyToMeanAnomaly(double EA, double e) {
        return EA - e * sin(EA);
    }


}
