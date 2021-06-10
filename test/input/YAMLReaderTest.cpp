#include "gtest/gtest.h"

#include <array>
#include "input/YAMLReader.h"
#include "model/SatelliteCollection.h"


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
    }

    std::array<Satellite, 4> _sat{};
};

/**
 * Checks if the file "resources/getSatelliteCollectionTest.yaml" is correctly read in
 */
TEST_F(YAMLReaderTest, getSatelliteCollectionTest) {
    YAMLReader yamlReader{"resources/getSatelliteCollectionTest.yaml"};
    SatelliteCollection satelliteCollection = yamlReader.getSatelliteCollection();

    ASSERT_EQ(satelliteCollection.size(), 4);

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
