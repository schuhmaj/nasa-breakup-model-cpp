#include "OrbitalElements.h"

std::array<double, 6> OrbitalElements::getAsArray() const {
    return std::array<double, 6>{_semiMajorAxis, _eccentricity, _inclination,
                                 _longitudeOfTheAscendingNode, _argumentOfPeriapsis, _eccentricAnomaly};
}

double OrbitalElements::operator[](size_t index) const {
    switch (index) {
        case 0:
            return _semiMajorAxis;
        case 1:
            return _eccentricity;
        case 2:
            return _inclination;
        case 3:
            return _longitudeOfTheAscendingNode;
        case 4:
            return _argumentOfPeriapsis;
        case 5:
            return _eccentricAnomaly;
        default:
            throw std::out_of_range{"The Orbital elements only consists of six elements --> range[0;5]!"};
    }
}

double OrbitalElements::getSemiMajorAxis() const {
    return _semiMajorAxis;
}

double OrbitalElements::getEccentricity() const {
    return _eccentricity;
}

double OrbitalElements::getInclination(AngularUnit angularUnit) const {
    return convertAngle(_inclination, angularUnit);
}

double OrbitalElements::getLongitudeOfTheAscendingNode(AngularUnit angularUnit) const {
    return convertAngle(_longitudeOfTheAscendingNode, angularUnit);
}

double OrbitalElements::getArgumentOfPeriapsis(AngularUnit angularUnit) const {
    return convertAngle(_argumentOfPeriapsis, angularUnit);
}

double OrbitalElements::getAnomaly(AngularUnit angularUnit, OrbitalAnomalyType anomalyType) const {
    return convertEccentricAnomaly(_eccentricAnomaly, _eccentricity, angularUnit, anomalyType);
}

bool operator==(const OrbitalElements &lhs, const OrbitalElements &rhs) {
    return lhs._semiMajorAxis == rhs._semiMajorAxis &&
           lhs._eccentricity == rhs._eccentricity &&
           lhs._inclination == rhs._inclination &&
           lhs._longitudeOfTheAscendingNode == rhs._longitudeOfTheAscendingNode &&
           lhs._argumentOfPeriapsis == rhs._argumentOfPeriapsis &&
           lhs._eccentricAnomaly == rhs._eccentricAnomaly;
}

bool operator!=(const OrbitalElements &lhs, const OrbitalElements &rhs) {
    return !(rhs == lhs);
}


double OrbitalElements::convertAngle(double angle, AngularUnit targetAngularUnit) {
    return targetAngularUnit == AngularUnit::RADIAN ? angle : util::radToDeg(angle);
}

double OrbitalElements::convertEccentricAnomaly(double anomaly, double eccentricity, AngularUnit targetAngularUnit,
                                                OrbitalAnomalyType targetOrbitalAnomalyType) {
    switch (targetOrbitalAnomalyType) {
        case OrbitalAnomalyType::ECCENTRIC:
            break;
        case OrbitalAnomalyType::MEAN:
            anomaly = util::eccentricAnomalyToMeanAnomaly(anomaly, eccentricity);

            break;
        case OrbitalAnomalyType::TRUE:
            anomaly = util::eccentricAnomalyToTrueAnomaly(anomaly, eccentricity);
            break;
    }
    if (targetAngularUnit == AngularUnit::DEGREE) {
        anomaly = util::radToDeg(anomaly);
    }
    return anomaly;
}
