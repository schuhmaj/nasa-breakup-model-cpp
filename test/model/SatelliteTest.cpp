#include "gtest/gtest.h"

#include "model/Satellite.h"


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
