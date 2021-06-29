#include "gtest/gtest.h"

#include <string>
#include <array>
#include <utility>
#include "input/TLEReader.h"

class TLEReaderTest : public ::testing::Test {

protected:

    size_t _expectedID_1 = 25544;
    std::array<double, 6> _expectedKepler_1 = {15.72125391, 0.0006703, 51.6416,
                                            247.4627, 130.5360, 325.0288};

    size_t _expectedID_2 = 48808;
    std::array<double, 6> _expectedKepler_2 = {1.00272877, 0.0000694, 0.0541,
                                            226.6478, 252.0694, 256.3446};

};

TEST_F(TLEReaderTest, readTLE_01_Test) {
    TLEReader tleReader{"resources/test-tle1.txt"};

    auto id_kepler = tleReader.getIDKepler();

    ASSERT_EQ(id_kepler.size(), 1);

    auto actualID = std::get<0>(id_kepler.at(0));
    auto actualKepler = std::get<1>(id_kepler.at(0));

    ASSERT_EQ(actualID, _expectedID_1);
    ASSERT_EQ(actualKepler, _expectedKepler_1);
}

TEST_F(TLEReaderTest, readTLE_02_Test) {
    TLEReader tleReader{"resources/test-tle2.txt"};

    auto id_kepler = tleReader.getIDKepler();

    ASSERT_EQ(id_kepler.size(), 1);

    auto actualID = std::get<0>(id_kepler.at(0));
    auto actualKepler = std::get<1>(id_kepler.at(0));

    ASSERT_EQ(actualID, _expectedID_2);
    ASSERT_EQ(actualKepler, _expectedKepler_2);
}

TEST_F(TLEReaderTest, readTLE_03_Test) {
    TLEReader tleReader{"resources/test-tle3.txt"};

    auto id_kepler = tleReader.getIDKepler();

    ASSERT_EQ(id_kepler.size(), 2);

    auto actualID = std::get<0>(id_kepler.at(0));
    auto actualKepler = std::get<1>(id_kepler.at(0));

    ASSERT_EQ(actualID, _expectedID_1);
    ASSERT_EQ(actualKepler, _expectedKepler_1);

    actualID = std::get<0>(id_kepler.at(1));
    actualKepler = std::get<1>(id_kepler.at(1));

    ASSERT_EQ(actualID, _expectedID_2);
    ASSERT_EQ(actualKepler, _expectedKepler_2);
}

