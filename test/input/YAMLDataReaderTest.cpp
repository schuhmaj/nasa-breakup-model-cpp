#include "gtest/gtest.h"

#include <array>
#include "input/YAMLDataReader.h"


class YAMLReaderTest : public ::testing::Test {

protected:
    virtual void SetUp() {
        SatelliteBuilder satelliteBuilder{};

        _sat[0] = satelliteBuilder
                .setID(1)
                .setName("Iridium 33")
                .setSatType(SatType::SPACECRAFT)
                .setMass(700)
                .setVelocity({1,1,1})
                .getResult();

        _sat[1] = satelliteBuilder
                .setID(2)
                .setName("Kosmos 2251")
                .setSatType(SatType::SPACECRAFT)
                .setMass(900)
                .setVelocity({2,2,2})
                .getResult();

        _sat[2] = satelliteBuilder
                .setID(3)
                .setName("1962-060B")
                .setSatType(SatType::ROCKET_BODY)
                .setMassByArea(3.5428)
                .setVelocity({2,2,2})
                .getResult();

        _sat[3] = satelliteBuilder
                .setID(4)
                .setName("UFO")
                .setSatType(SatType::UNKNOWN)
                .setMassByArea(3333.33)
                .setVelocity({11,22,33})
                .getResult();
        _sat[4] = satelliteBuilder
                .setID(25544)
                .setName("ISS")
                .setSatType(SatType::SPACECRAFT)
                .setMassByArea(399.05)
                .setKeplerianElementsMA({6798505.86, 0.0002215, 0.9013735469,
                                         4.724103630312, 2.237100203348, 0.2405604761})
                .getResult();
    }

    std::array<Satellite, 5> _sat{};
};

/**
 * Checks if the file "resources/getSatelliteCollectionTest.yaml" is correctly read in
 */
TEST_F(YAMLReaderTest, getSatelliteCollectionTest) {
    YAMLDataReader yamlReader{"resources/getSatelliteCollectionTest.yaml"};
    auto satelliteCollection = yamlReader.getSatelliteCollection();

    ASSERT_EQ(satelliteCollection.size(), 5);

    auto expectedSatellites = _sat.begin();
    for(auto &sat : satelliteCollection) {
        //== in Satellite only checks for ID equality
        ASSERT_EQ(sat, *expectedSatellites);

        //Checking the other attributes for equality
        ASSERT_EQ(sat.getName(), expectedSatellites->getName());
        ASSERT_EQ(sat.getSatType(), expectedSatellites->getSatType());
        ASSERT_NEAR(sat.getMass(), expectedSatellites->getMass(), 0.0001);
        ASSERT_NEAR(sat.getArea(), expectedSatellites->getArea(), 0.0001);
        ASSERT_NEAR(sat.getCharacteristicLength(), expectedSatellites->getCharacteristicLength(), 0.0001);
        ASSERT_EQ(sat.getVelocity(), expectedSatellites->getVelocity());
        ASSERT_EQ(sat.getPosition(), expectedSatellites->getPosition());

        ++expectedSatellites;
    }

}
