#include "gtest/gtest.h"

#include <utility>
#include "util/UtilityKepler.h"


class UtilityKeplerTest : public ::testing::TestWithParam<double> {

};

TEST_P(UtilityKeplerTest, meanAnomalyToEccentricAnomaly){
    using namespace util;
    double eccentricity = 0.5;

    double expectedMeanAnomaly = GetParam();

    double actualEccentricAnomaly = meanAnomalyToEccentricAnomaly(expectedMeanAnomaly, eccentricity);

    double actualMeanAnomaly = eccentricAnomalyToMeanAnomaly(actualEccentricAnomaly, eccentricity);

    ASSERT_DOUBLE_EQ(actualMeanAnomaly, expectedMeanAnomaly);
}

INSTANTIATE_TEST_SUITE_P(DoubleParam, UtilityKeplerTest,
                         ::testing::Values(5.673, 3.14, 0.25, 1.0));
