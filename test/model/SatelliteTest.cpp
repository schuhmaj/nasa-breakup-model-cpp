#include "gtest/gtest.h"

#include "model/Satellite.h"
#include <array>

/**
 * Ensure that that all arguments are correctly set-up (set to zero expect ID)
 */
TEST(SatelliteTest, Constructor) {
    Satellite sat{1};
    std::array<double, 3> zeroArray{ {0.0, 0.0, 0.0} };

    EXPECT_EQ(sat.getId(), 1);
    EXPECT_EQ(sat.getArea(), 0.0);
    EXPECT_EQ(sat.getAreaToMassRatio(), 0.0);
    EXPECT_EQ(sat.getCharacteristicLength(), 0.0);
    EXPECT_EQ(sat.getMass(), 0.0);
    EXPECT_EQ(sat.getName(), "");
    EXPECT_EQ(sat.getVelocity(), zeroArray);
    EXPECT_EQ(sat.getPosition(), zeroArray);
}

TEST(SatelliteTest, operatorEQTest) {
    Satellite sat1{1};
    Satellite sat2{2};

    ASSERT_FALSE(sat1 == sat2);
}

TEST(SatelliteTest, operatorNEQTest) {
    Satellite sat1{1};
    Satellite sat2{2};

    ASSERT_TRUE(sat1 != sat2);
}
