#include "gtest/gtest.h"

#include "model/Satellite.h"
#include "util/UtilityKepler.h"
#include <array>
#include <utility>

class SatelliteTest : public ::testing::Test{

};

/**
 * Ensure that that all arguments are correctly set-up (set to zero expect ID)
 */
TEST_F(SatelliteTest, Constructor) {
    Satellite sat{1};
    std::array<double, 3> zeroArray{{0.0, 0.0, 0.0}};

    EXPECT_EQ(sat.getId(), 1);
    EXPECT_EQ(sat.getArea(), 0.0);
    EXPECT_EQ(sat.getAreaToMassRatio(), 0.0);
    EXPECT_EQ(sat.getCharacteristicLength(), 0.0);
    EXPECT_EQ(sat.getMass(), 0.0);
    EXPECT_EQ(sat.getName(), "");
    EXPECT_EQ(sat.getVelocity(), zeroArray);
    EXPECT_EQ(sat.getPosition(), zeroArray);
}

TEST_F(SatelliteTest, operatorEQTest) {
    Satellite sat1{1};
    Satellite sat2{2};

    ASSERT_FALSE(sat1 == sat2);
}

TEST_F(SatelliteTest, operatorNEQTest) {
    Satellite sat1{1};
    Satellite sat2{2};

    ASSERT_TRUE(sat1 != sat2);
}

TEST_F(SatelliteTest, KeplerConversionTest) {
    std::array<double, 6> expectedKepler = {15.72125391, 0.0006703, 51.6416,
                                               247.4627, 130.5360, 325.0288};
    expectedKepler[0] = util::meanMotionToSemiMajorAxis(expectedKepler[0]);
    expectedKepler[2] = util::degToRad(expectedKepler[2]);
    expectedKepler[3] = util::degToRad(expectedKepler[3]);
    expectedKepler[4] = util::degToRad(expectedKepler[4]);
    expectedKepler[5] = util::degToRad(expectedKepler[5]);
    Satellite sat{1};

    sat.setCartesianByKeplerMA(expectedKepler);

    auto actualKepler = sat.getKeplerMA();

    for (unsigned int i = 0; i < 6; ++i) {
        EXPECT_NEAR(actualKepler[i], expectedKepler[i], 0.0001);
    }
}
