#include "OrbitalElementsFactory.h"

OrbitalElements OrbitalElementsFactory::createFromTLEData(const std::array<double, 6> &tleData) const {
    auto degKepler = tleData;
    degKepler[0] = util::meanMotionToSemiMajorAxis(degKepler[0]);
    return createFromOnlyDegree(degKepler, OrbitalAnomalyType::MEAN);
}

OrbitalElements OrbitalElementsFactory::createFromOnlyRadians(const std::array<double, 6> &standardKepler,
                                                              OrbitalAnomalyType orbitalAnomalyType) const {
    double anomaly = standardKepler[5];
    if (orbitalAnomalyType == OrbitalAnomalyType::MEAN) {
        anomaly = util::meanAnomalyToEccentricAnomaly(anomaly, standardKepler[1]);
    } else if (orbitalAnomalyType == OrbitalAnomalyType::TRUE) {
        anomaly = util::trueAnomalyToEccentricAnomaly(anomaly, standardKepler[1]);
    }   //else ECCENTRIC --> right format

    return OrbitalElements(standardKepler[0], standardKepler[1], standardKepler[2],
                           standardKepler[3], standardKepler[4], anomaly);
}

OrbitalElements OrbitalElementsFactory::createFromOnlyDegree(const std::array<double, 6> &standardKepler,
                                                             OrbitalAnomalyType orbitalAnomalyType) const {
    auto radKepler = standardKepler;
    for (unsigned int i = 2; i < 6; ++i) {
        radKepler[i] = util::degToRad(radKepler[i]);
    }
    return this->createFromOnlyRadians(radKepler, orbitalAnomalyType);
}
