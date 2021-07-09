#include "OrbitalElements.h"

std::array<double, 6> OrbitalElements::getAsUniform() const {
    std::array<double, 6> keplerElements{};
    for (unsigned int i = 0; i < 5; ++i) {
        keplerElements[i] = (*this)[i];
    }
    return keplerElements;
}

double OrbitalElements::operator[](size_t index) const {
    switch (index) {
        case 0:
            return getSemiMajorAxis();
        case 1:
            return getEccentricity();
        case 2:
            return getInclination(AngularUnit::RADIAN);
        case 3:
            return getLongitudeOfTheAscendingNode(AngularUnit::RADIAN);
        case 4:
            return getArgumentOfPeriapsis(AngularUnit::RADIAN);
        case 5:
            return getAnomaly(AngularUnit::RADIAN, OrbitalAnomalyType::ECCENTRIC);
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
    return convertAnomaly(_anomaly, angularUnit, anomalyType);
}

double OrbitalElements::convertAngle(const std::pair<double, AngularUnit> &angle, AngularUnit targetAngularUnit) const {
    if (targetAngularUnit == angle.second) {
        return angle.first;
    } else {
        if (angle.second == AngularUnit::DEGREE) {
            return util::degToRad(angle.first);
        } else {
            return util::radToDeg(angle.first);
        }
    }
}

double OrbitalElements::convertAnomaly(const std::tuple<double, AngularUnit, OrbitalAnomalyType> &anomaly,
                                       AngularUnit targetAngularUnit,
                                       OrbitalAnomalyType targetOrbitalAnomalyType) const {
    double angle = std::get<0>(anomaly);
    AngularUnit angularUnit = std::get<1>(anomaly);
    OrbitalAnomalyType anomalyType = std::get<2>(anomaly);

    //First check if we already have the result or if we only have to convert angular units
    if (anomalyType == targetOrbitalAnomalyType) {
        if (angularUnit == AngularUnit::DEGREE && targetAngularUnit == AngularUnit::RADIAN) {
            return util::degToRad(angle);
        } else if (angularUnit == AngularUnit::RADIAN && targetAngularUnit == AngularUnit::DEGREE) {
            return util::radToDeg(angle);
        } else {
            return angle;
        }
    }

    //angle is now definitely in [RAD]
    angle = convertAngle(std::make_pair(angle, angularUnit), AngularUnit::RADIAN);

    //Convert angle of anomaly to eccentric anomaly angle, if it is eccentric we do nothing
    if (anomalyType == OrbitalAnomalyType::TRUE) {
        angle = util::trueAnomalyToEccentricAnomaly(angle, _eccentricity);
    } else if (anomalyType == OrbitalAnomalyType::MEAN) {
        angle = util::meanAnomalyToEccentricAnomaly(angle, _eccentricity);
    }

    //Convert anomaly into target anomaly type; anomaly is already eccentric, if this is the right target, do nothing
    if (targetOrbitalAnomalyType == OrbitalAnomalyType::TRUE) {
        angle = util::eccentricAnomalyToTrueAnomaly(angle, _eccentricity);
    } else if (targetOrbitalAnomalyType == OrbitalAnomalyType::MEAN) {
        angle = util::eccentricAnomalyToMeanAnomaly(angle, _eccentricity);
    }

    //Convert angle into target unit; angle is already in [RAD], if [RAD] is the target unit, do nothing
    if (targetAngularUnit == AngularUnit::DEGREE) {
        angle = util::radToDeg(angle);
    }
    return angle;
}


