#include "gtest/gtest.h"

#include "model/Satellite.h"
#include "model/OrbitalElementsFactory.h"
#include "model/OrbitalElements.h"
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
    OrbitalElementsFactory factory{};
    auto expectedKepler = factory.createFromTLEData({15.72125391, 0.0006703, 51.6416,
                                                     247.4627, 130.5360, 325.0288});
    Satellite sat{1};
    sat.setCartesianByOrbitalElements(expectedKepler);


    auto actualKepler = sat.getOrbitalElements();

    for (unsigned int i = 0; i < 6; ++i) {
        EXPECT_NEAR(actualKepler[i], expectedKepler[i], 0.0001) << "i=" << i;
    }
}

class SatelliteOrbitalElementsTest : public ::testing::TestWithParam<OrbitalElements> {

};

std::vector<OrbitalElements> getOrbitalParameters() {
    OrbitalElementsFactory factory{};
    std::vector<OrbitalElements> values{};

    //Why is the 0. value not in the vector?
    //If eccentricity is zero and inclination too, then W and w have no influence/ are not unique
    std::array<double, 6> kepler{{6800000.0, 0.0, 0.0,
                                         0.0, 0.0, 1.5708}};

    for (unsigned int i = 0; i < 50; ++i) {
        kepler[0] += 100000;        //+100km
        kepler[1] += 0.01;          //+0.01 eccentricity
        kepler[2] += 0.0349066;     //+2 deg inclination
        kepler[3] += 0.0174533;     //+1 deg RAAN
        kepler[4] += 0.00872665;    //+0.5 deg argument of perigee
        values.push_back(factory.createFromOnlyRadians(kepler, OrbitalAnomalyType::MEAN));
    }

    return values;
}

TEST_P(SatelliteOrbitalElementsTest, KeplerConversionTest) {
    auto expectedKepler = GetParam();
    Satellite sat{1};

    sat.setCartesianByOrbitalElements(expectedKepler);

    auto actualKepler = sat.getOrbitalElements();

    for (unsigned int i = 0; i < 6; ++i) {
        EXPECT_NEAR(actualKepler[i], expectedKepler[i], 0.0001) << "i = " << i;
    }
}

INSTANTIATE_TEST_SUITE_P(KeplerArgumentParam, SatelliteOrbitalElementsTest,
                         ::testing::ValuesIn(getOrbitalParameters()));