#include "gtest/gtest.h"

#include <vector>
#include "model/Satellite.h"
#include "model/SatelliteCollection.h"


class SatelliteCollectionTest : public ::testing::Test {

protected:

    virtual void SetUp() {
        _satVector.push_back(_sat1);
        _satVector.push_back(_sat2);
        _satVector.push_back(_sat3);
        _satVector.push_back(_sat4);
    }

    virtual void TearDown() {
        _satVector.clear();
    }

    Satellite _sat1{1};
    Satellite _sat2{2};
    Satellite _sat3{3};
    Satellite _sat4{4};
    std::vector<Satellite> _satVector;

};

/**
 * Tests the push_back methods
 */
TEST_F(SatelliteCollectionTest, push_back) {
    SatelliteCollection satelliteCollection{};
    satelliteCollection.push_back(_sat1);
    satelliteCollection.push_back(_sat2);
    satelliteCollection.push_back(_sat3);
    satelliteCollection.push_back(Satellite{5});
    ASSERT_EQ(satelliteCollection.size(), 4);
}

/**
 * Checks if remove and its internal use of swap are working
 */
TEST_F(SatelliteCollectionTest, remove) {
    SatelliteCollection satelliteCollection{_satVector};
    ASSERT_EQ(satelliteCollection.size(), 4);

    satelliteCollection.remove(_sat2);

    ASSERT_EQ(satelliteCollection.size(), 3);

    ASSERT_EQ(satelliteCollection[0], _sat1);
    ASSERT_EQ(satelliteCollection[1], _sat4);
    ASSERT_EQ(satelliteCollection[2], _sat3);
}

/**
 * Checks the iterator
 */
TEST_F(SatelliteCollectionTest, Iterator) {
    SatelliteCollection satelliteCollection{_satVector};
    int i = 0;
    for(auto& sat: satelliteCollection) {
        ASSERT_EQ(sat, _satVector[i++]);
    }
}
