#include "Satellite.h"

std::ostream &operator<<(std::ostream &os, SatType satType) {
    return os << Satellite::satTypeToString.at(satType);
}

const std::map<std::string, SatType> Satellite::stringToSatType{{"SPACECRAFT",  SatType::SPACECRAFT},
                                                                {"SC",          SatType::SPACECRAFT},
                                                                {"ROCKET_BODY", SatType::ROCKET_BODY},
                                                                {"RB",          SatType::ROCKET_BODY},
                                                                {"DEBRIS",      SatType::DEBRIS},
                                                                {"DEB",         SatType::DEBRIS},
                                                                {"UNKNOWN",     SatType::UNKNOWN}
};

const std::map<SatType, std::string> Satellite::satTypeToString{{SatType::SPACECRAFT,  "SPACECRAFT"},
                                                                {SatType::ROCKET_BODY, "ROCKET_BODY"},
                                                                {SatType::DEBRIS,      "DEBRIS"},
                                                                {SatType::UNKNOWN,     "UNKNOWN"}
};

void Satellite::keplerToCartesianEA(double a, double e, double i, double W, double w, double EA) {
    using util::GRAVITATIONAL_PARAMETER_EARTH;
    double b, n, xper, yper, xdotper, ydotper;
    double R[3][3];
    double cosomg, cosomp, sinomg, sinomp, cosi, sini;
    double dNdZeta;

    // semi-major axis is assumed to be positive here we apply the convention of having it negative as for
    // computations to result in higher elegance
    if (e > 1) {
        a = -a;
    }

    // 1 - We start by evaluating position and velocity in the perifocal reference system
    if (e < 1.0) // EA is the eccentric anomaly
    {
        b = a * sqrt(1 - e * e);
        n = sqrt(GRAVITATIONAL_PARAMETER_EARTH / (a * a * a));

        xper = a * (cos(EA) - e);
        yper = b * sin(EA);
        xdotper = -(a * n * sin(EA)) / (1 - e * cos(EA));
        ydotper = (b * n * cos(EA)) / (1 - e * cos(EA));
    } else // EA is the Gudermannian
    {
        b = -a * sqrt(e * e - 1);
        n = sqrt(-GRAVITATIONAL_PARAMETER_EARTH / (a * a * a));

        dNdZeta = e * (1 + tan(EA) * tan(EA)) - (0.5 + 0.5 * pow(tan(0.5 * EA + M_PI_4), 2)) / tan(0.5 * EA + M_PI_4);

        xper = a / cos(EA) - a * e;
        yper = b * tan(EA);

        xdotper = a * tan(EA) / cos(EA) * n / dNdZeta;
        ydotper = b / pow(cos(EA), 2) * n / dNdZeta;
    }

    // 2 - We then built the rotation matrix from perifocal reference frame to inertial

    cosomg = cos(W);
    cosomp = cos(w);
    sinomg = sin(W);
    sinomp = sin(w);
    cosi = cos(i);
    sini = sin(i);

    R[0][0] = cosomg * cosomp - sinomg * sinomp * cosi;
    R[0][1] = -cosomg * sinomp - sinomg * cosomp * cosi;
    R[0][2] = sinomg * sini;
    R[1][0] = sinomg * cosomp + cosomg * sinomp * cosi;
    R[1][1] = -sinomg * sinomp + cosomg * cosomp * cosi;
    R[1][2] = -cosomg * sini;
    R[2][0] = sinomp * sini;
    R[2][1] = cosomp * sini;
    R[2][2] = cosi;

    // 3 - We end by transforming according to this rotation matrix

    double temp[3] = {xper, yper, 0.0};
    double temp2[3] = {xdotper, ydotper, 0};

    for (int j = 0; j < 3; j++) {
        _position[j] = 0.0;
        _velocity[j] = 0.0;
        for (int k = 0; k < 3; k++) {
            _position[j] += R[j][k] * temp[k];
            _velocity[j] += R[j][k] * temp2[k];
        }
    }
}

void Satellite::keplerToCartesianMA(double a, double e, double i, double W, double w, double MA) {
    double EA = util::meanAnomalyToEccentricAnomaly(MA, e);
    keplerToCartesianEA(a, e, i, W, w, EA);
}

std::ostream &operator<<(std::ostream &os, const Satellite &satellite) {
    //TODO Rework later
    return os << "ID:\t" << satellite._id << "\n"
              << "Name:\t" << satellite._name << "\n"
              << "L_c:\t" << satellite._characteristicLength << "\n"
              << "M:\t" << satellite._mass << "\n"
              << "A/M:\t" << satellite._areaToMassRatio << "\n";
}
