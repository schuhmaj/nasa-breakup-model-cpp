#include "Satellite.h"

std::ostream &operator<<(std::ostream &os, SatType satType) {
    return os << Satellite::satTypeToString.at(satType);
}

std::istream &operator>>(std::istream &istream, SatType &satType) {
    std::string string;
    istream >> string;
    satType = Satellite::stringToSatType.at(string);
    return istream;
}

const std::map<std::string, SatType> Satellite::stringToSatType{{"SPACECRAFT",  SatType::SPACECRAFT},
                                                                {"SC",          SatType::SPACECRAFT},
                                                                {"PAY",          SatType::SPACECRAFT},
                                                                {"ROCKET_BODY", SatType::ROCKET_BODY},
                                                                {"RB",          SatType::ROCKET_BODY},
                                                                {"R/B",          SatType::ROCKET_BODY},
                                                                {"DEBRIS",      SatType::DEBRIS},
                                                                {"DEB",         SatType::DEBRIS},
                                                                {"UNKNOWN",     SatType::UNKNOWN},
                                                                {"UNK",         SatType::UNKNOWN}
};

const std::map<SatType, std::string> Satellite::satTypeToString{{SatType::SPACECRAFT,  "SPACECRAFT"},
                                                                {SatType::ROCKET_BODY, "ROCKET_BODY"},
                                                                {SatType::DEBRIS,      "DEBRIS"},
                                                                {SatType::UNKNOWN,     "UNKNOWN"}
};

void Satellite::setCartesianByKeplerEA(const std::array<double, 6> &keplerianElements) {
    using util::GRAVITATIONAL_PARAMETER_EARTH;

    auto &E = keplerianElements;
    double a = E[0];
    double e = E[1];
    double i = E[2];
    double omg = E[3];
    double omp = E[4];
    double EA = E[5];
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
        n = sqrt((- GRAVITATIONAL_PARAMETER_EARTH) / (a * a * a));

        dNdZeta = e * (1 + tan(EA) * tan(EA)) - (0.5 + 0.5 * pow(tan(0.5 * EA + M_PI_4), 2)) / tan(0.5 * EA + M_PI_4);

        xper = a / cos(EA) - a * e;
        yper = b * tan(EA);

        xdotper = a * tan(EA) / cos(EA) * n / dNdZeta;
        ydotper = b / pow(cos(EA), 2) * n / dNdZeta;
    }

    // 2 - We then built the rotation matrix from perifocal reference frame to inertial

    cosomg = cos(omg);
    cosomp = cos(omp);
    sinomg = sin(omg);
    sinomp = sin(omp);
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

    auto &r0 = _position;
    auto &v0 = _velocity;
    for (int j = 0; j < 3; j++) {
        r0[j] = 0.0;
        v0[j] = 0.0;
        for (int k = 0; k < 3; k++) {
            r0[j] += R[j][k] * temp[k];
            v0[j] += R[j][k] * temp2[k];
        }
    }
}

void Satellite::setCartesianByKeplerMA(const std::array<double, 6> &keplerianElements) {
    std::array<double, 6> keplerianElementsEA {keplerianElements};
    keplerianElementsEA[5] = util::meanAnomalyToEccentricAnomaly(keplerianElements[5], keplerianElements[1]);
    this->setCartesianByKeplerEA(keplerianElementsEA);
}

void Satellite::setCartesianByKeplerTA(const std::array<double, 6> &keplerianElements) {
    std::array<double, 6> keplerianElementsEA {keplerianElements};
    keplerianElementsEA[5] = util::trueAnomalyToEccentricAnomaly(keplerianElements[5], keplerianElements[1]);
    this->setCartesianByKeplerEA(keplerianElementsEA);
}

void Satellite::setCartesianByKeplerTLEFormat(const std::array<double, 6> &keplerianElements) {
    std::array<double, 6> keplerianElementsMA {keplerianElements};
    keplerianElementsMA[0] = util::meanMotionToSemiMajorAxis(keplerianElements[0]);
    keplerianElementsMA[2] = util::degToRad(keplerianElements[0]);
    keplerianElementsMA[3] = util::degToRad(keplerianElements[0]);
    keplerianElementsMA[4] = util::degToRad(keplerianElements[0]);
    keplerianElementsMA[5] = util::degToRad(keplerianElements[0]);
    this->setCartesianByKeplerMA(keplerianElementsMA);
}

std::array<double, 6> Satellite::getKeplerEA() {
    using namespace util;
    std::array<double, 6> keplerianElements{};

    std::array<double, 3> k = {0.0, 0.0, 1.0};
    // build generic arrays to size - init values don't matter:
    std::array<double, 3> h = {0.0, 0.0, 0.0};
    std::array<double, 3> Dum_Vec = {0.0, 0.0, 0.0};
    std::array<double, 3> n = {0.0, 0.0, 0.0};
    std::array<double, 3> evett = {0.0, 0.0, 0.0};

    double p = 0.0;
    double temp = 0.0;
    double R0, ni;
    int i;

    /// 1 - We compute h: the orbital angular momentum vector
    h = cross(_position, _velocity);

    /// 2 - We compute p: the orbital parameter
    p = dot(h, h) / GRAVITATIONAL_PARAMETER_EARTH; // h^2 / mu

    /// 3 - We compute n: the vector of the node line
    /// This operation is singular when inclination is zero, in which case the orbital parameters
    /// are not defined
    n = cross(k, h);
    n = n / euclideanNorm(n); // vers(x, y) = unit vector of y -> x

    /// 4 - We compute evett: the eccentricity vector
    R0 = euclideanNorm(_position);
    Dum_Vec = cross(_velocity, h);
    evett = Dum_Vec / GRAVITATIONAL_PARAMETER_EARTH - _position / R0;

    /// The eccentricity is calculated and stored as the second orbital element
    keplerianElements[1] = euclideanNorm(evett);

    /// The semi-major axis (positive quantity) is calculated and stored as the first orbital element
    keplerianElements[0] = std::abs(p / (1 - keplerianElements[1] * keplerianElements[1]));

    /// Inclination is calculated and stored as the third orbital element
    keplerianElements[2] = acos(h[2] / euclideanNorm(h));

    /// Argument of pericentrum is calculated and stored as the fifth orbital element
    temp = dot(n, evett);
    keplerianElements[4] = acos(temp / keplerianElements[1]);
    if (evett[2] < 0) {
        keplerianElements[4] = 2 * PI - keplerianElements[4];
    }

    /// Argument of longitude is calculated and stored as the fourth orbital element
    keplerianElements[3] = acos(n[0]);
    if (n[1] < 0) {
        keplerianElements[3] = 2 * PI - keplerianElements[3];
    }

    temp = dot(evett, _position);

    /// 4 - We compute ni: the true anomaly (in 0, 2*PI)
    ni = acos(temp / keplerianElements[1] / R0);

    temp = dot(_position, _velocity);

    if (temp < 0.0) {
        ni = 2 * PI - ni;
    }

    /// Eccentric anomaly or the gudermannian is calculated and stored as the sixth orbital element
    if (keplerianElements[1] < 1.0) {
        keplerianElements[5] = 2.0 * atan(sqrt((1 - keplerianElements[1]) / (1 + keplerianElements[1])) *
                                          tan(ni / 2.0)); // algebraic kepler's equation
    } else {
        keplerianElements[5] = 2.0 * atan(sqrt((keplerianElements[1] - 1) / (keplerianElements[1] + 1))
                                          * tan(ni /
                                                2.0)); // algebraic equivalent of kepler's equation in terms of the Gudermannian
    }
    if (keplerianElements[5] < 0) {
        keplerianElements[5] += 2 * PI;
    }
    return keplerianElements;
}

std::array<double, 6> Satellite::getKeplerMA() {
    auto kepler = getKeplerEA();
    kepler[5] = util::eccentricAnomalyToMeanAnomaly(kepler[5], kepler[1]);
    return kepler;
}

std::array<double, 6> Satellite::getKeplerTA() {
    auto kepler = getKeplerEA();
    kepler[5] = util::eccentricAnomalyToTrueAnomaly(kepler[5], kepler[1]);
    return kepler;
}
