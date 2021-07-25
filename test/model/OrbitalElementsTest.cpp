#include "gtest/gtest.h"

#include "model/OrbitalElements.h"
#include "model/OrbitalElementsFactory.h"

TEST(OrbitalElementsTest, OrbitalElements01) {
    OrbitalElementsFactory factory{};

    double expectedSemiMajorAxis = 42165260.2513;
    double expectedEccentricity = 0.00006880;
    double expectedInclination = 89.76;
    double expectedRAAN = 1.22222;
    double expectedArgOfPer = 341.0477201;
    double expectedTrueAnomaly = 184.6690779;

    OrbitalElements actualOrbitalElements =
            factory.createOrbitalElements(expectedSemiMajorAxis,
                                          expectedEccentricity,
                                          expectedInclination, AngularUnit::DEGREE,
                                          expectedRAAN, AngularUnit::RADIAN,
                                          expectedArgOfPer, AngularUnit::DEGREE,
                                          expectedTrueAnomaly, AngularUnit::DEGREE, OrbitalAnomalyType::TRUE);

    ASSERT_DOUBLE_EQ(expectedSemiMajorAxis, actualOrbitalElements.getSemiMajorAxis());
    ASSERT_DOUBLE_EQ(expectedEccentricity, actualOrbitalElements.getEccentricity());
    ASSERT_DOUBLE_EQ(expectedInclination, actualOrbitalElements.getInclination(AngularUnit::DEGREE));
    ASSERT_DOUBLE_EQ(expectedRAAN, actualOrbitalElements.getLongitudeOfTheAscendingNode(AngularUnit::RADIAN));
    ASSERT_DOUBLE_EQ(expectedArgOfPer, actualOrbitalElements.getArgumentOfPeriapsis(AngularUnit::DEGREE));
    ASSERT_DOUBLE_EQ(expectedTrueAnomaly,
                     actualOrbitalElements.getAnomaly(AngularUnit::DEGREE, OrbitalAnomalyType::TRUE));

}

TEST(OrbitalElementsTest, OrbitalElements02) {
    OrbitalElementsFactory factory{};

    double expectedSemiMajorAxis = 42165260.2513;
    double expectedEccentricity = 0.00006880;
    double expectedInclination = 89.76;
    double expectedRAAN = 1.22222;
    double expectedArgOfPer = 341.0477201;
    double expectedTrueAnomaly = 184.6690779;

    OrbitalElements actualOrbitalElements =
            factory.createFromOnlyDegree({expectedSemiMajorAxis, expectedEccentricity,
                                          expectedInclination, expectedRAAN,
                                          expectedArgOfPer, expectedTrueAnomaly},
                                         OrbitalAnomalyType::ECCENTRIC);

    ASSERT_DOUBLE_EQ(expectedSemiMajorAxis, actualOrbitalElements.getSemiMajorAxis());
    ASSERT_DOUBLE_EQ(expectedEccentricity, actualOrbitalElements.getEccentricity());
    ASSERT_DOUBLE_EQ(expectedInclination, actualOrbitalElements.getInclination(AngularUnit::DEGREE));
    ASSERT_DOUBLE_EQ(expectedRAAN, actualOrbitalElements.getLongitudeOfTheAscendingNode(AngularUnit::DEGREE));
    ASSERT_DOUBLE_EQ(expectedArgOfPer, actualOrbitalElements.getArgumentOfPeriapsis(AngularUnit::DEGREE));
    ASSERT_DOUBLE_EQ(expectedTrueAnomaly, actualOrbitalElements.getAnomaly(AngularUnit::DEGREE));

}