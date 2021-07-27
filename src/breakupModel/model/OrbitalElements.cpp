#include "OrbitalElements.h"

std::array<double, 6> OrbitalElements::getAsArray() const {
    return std::array<double, 6>{_semiMajorAxis, _eccentricity, _inclination,
                                 _longitudeOfTheAscendingNode, _argumentOfPeriapsis, _eccentricAnomaly};
}

double OrbitalElements::operator[](size_t index) const {
    switch (index) {
        case OrbitalElement::SEMI_MAJOR_AXIS:
            return _semiMajorAxis;
        case OrbitalElement::ECCENTRICITY:
            return _eccentricity;
        case OrbitalElement::INCLINATION:
            return _inclination;
        case OrbitalElement::LONGITUDE_OF_THE_ASCENDING_NODE:
            return _longitudeOfTheAscendingNode;
        case OrbitalElement::ARGUMENT_OF_PERIAPSIS:
            return _argumentOfPeriapsis;
        case OrbitalElement::ECCENTRIC_ANOMALY:
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

std::ostream &operator<<(std::ostream &os, const OrbitalElements &elements) {
    os << "OrbitalElements{"
       << "_semiMajorAxis: " << elements._semiMajorAxis << " _eccentricity: " << elements._eccentricity
       << " _inclination: " << elements._inclination << " _longitudeOfTheAscendingNode: "
       << elements._longitudeOfTheAscendingNode << " _argumentOfPeriapsis: " << elements._argumentOfPeriapsis
       << " _eccentricAnomaly: " << elements._eccentricAnomaly << '}';
    return os;
}
