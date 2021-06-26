#include "gtest/gtest.h"

#include <utility>
#include <vector>
#include "util/UtilityFunctions.h"
#include "util/UtilityKepler.h"


class UtilityKeplerTest : public ::testing::TestWithParam<double> {

};

TEST_P(UtilityKeplerTest, meanAnomalyToEccentricAnomaly){
    using namespace util;
    double eccentricity = 0.5;

    double expectedMeanAnomaly = GetParam();

    double eccentricAnomaly = meanAnomalyToEccentricAnomaly(expectedMeanAnomaly, eccentricity);

    double actualMeanAnomaly = eccentricAnomalyToMeanAnomaly(eccentricAnomaly, eccentricity);

    ASSERT_DOUBLE_EQ(actualMeanAnomaly, expectedMeanAnomaly);
}

TEST_P(UtilityKeplerTest, trueAnomalyToEccentricAnomaly){
    using namespace util;
    double eccentricity = 0.5;

    double expectedTrueAnomaly = GetParam();

    double eccentricAnomaly = trueAnomalyToEccentricAnomaly(expectedTrueAnomaly, eccentricity);

    double actualTrueAnomaly = eccentricAnomalyToTrueAnomaly(eccentricAnomaly, eccentricity);

    ASSERT_DOUBLE_EQ(actualTrueAnomaly, expectedTrueAnomaly);
}

/**
 * Returns a vector containing all possible angles in [rad] in the interval [0.0, pi[ with spacing 0.1;
 * @return an vector containing 63 values from 0.0 to 3.1
 */
std::vector<double> radValues() {
    std::vector<double> val{};
    val.reserve(63);
    double d = 0.0;
    while (d < util::PI) {
        val.push_back(d);
        d += 0.1;
    }
    return val;
}

INSTANTIATE_TEST_SUITE_P(DoubleParam, UtilityKeplerTest,
                         ::testing::ValuesIn(radValues()));
