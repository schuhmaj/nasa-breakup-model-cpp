#include "gtest/gtest.h"

#include <string>
#include <map>
#include <array>
#include "input/TLESatcatDataReader.h"
#include "input/TLEReader.h"
#include "input/CSVReader.h"
#include "model/Satellite.h"
#include "model/SatelliteBuilder.h"

class TLESatcatDataReaderTest : public ::testing::Test {

protected:
    virtual void SetUp() {
        _satcat.emplace_back("SL-1 R/B", "1957-001A", 1, SatType::ROCKET_BODY, "D", "CIS", "1957-10-04", "TYMSC",
                             "1957-12-01", 96.19, 65.10, 938.0, 214.0, 20.4200, "", "EA", "IMP");
        _satcat.emplace_back("SPUTNIK 1", "1957-001B", 2, SatType::SPACECRAFT, "D", "CIS", "1957-10-04", "TYMSC",
                             "1958-01-03", 96.10, 65.00, 1080.0, 64.0, 0.0, "", "EA", "IMP");
        _satcat.emplace_back("SPUTNIK 2", "1957-002A", 3, SatType::SPACECRAFT, "D", "CIS", "1957-11-03", "TYMSC",
                             "1958-04-14", 103.74, 65.33, 1659.0, 211.0, 0.0800, "", "EA", "IMP");
        _satcat.emplace_back("EXPLORER 1", "1958-001A", 4, SatType::SPACECRAFT, "D", "US", "1958-02-01", "AFETR",
                             "1970-03-31", 88.48, 33.15, 215.0, 183.0, 0.0, "", "EA", "IMP");

        SatelliteBuilder satelliteBuilder{};
        _expectedSatellites.push_back(
                satelliteBuilder
                        .reset()
                        .setID(1)
                        .setName("SL-1 R/B")
                        .setSatType(SatType::ROCKET_BODY)
                        .setMassByArea(20.42)
                        .setKeplerianElementsTLEFormat(_expectedKepler)
                        .getResult()
        );
        _expectedSatellites.push_back(
                satelliteBuilder
                        .reset()
                        .setID(2)
                        .setName("SPUTNIK 1")
                        .setSatType(SatType::SPACECRAFT)
                        .setMassByArea(0)
                        .setKeplerianElementsTLEFormat(_expectedKepler)
                        .getResult()
        );
        _expectedSatellites.push_back(
                satelliteBuilder
                        .reset()
                        .setID(3)
                        .setName("SPUTNIK 2")
                        .setSatType(SatType::SPACECRAFT)
                        .setMassByArea(0.0800)
                        .setKeplerianElementsTLEFormat(_expectedKepler)
                        .getResult()
        );
        _expectedSatellites.push_back(
                satelliteBuilder
                        .reset()
                        .setID(4)
                        .setName("EXPLORER 1")
                        .setSatType(SatType::SPACECRAFT)
                        .setMassByArea(0)
                        .setKeplerianElementsTLEFormat(_expectedKepler)
                        .getResult()
        );

    }

    std::array<double, 6> _expectedKepler = {15.72125391, 0.0006703, 51.6416,
                                             247.4627, 130.5360, 325.0288};

    std::vector<std::tuple<std::string, std::string, size_t,
            SatType, std::string, std::string, std::string, std::string, std::string,
            double, double, double, double, double,
            std::string, std::string, std::string>> _satcat{};

    std::vector<Satellite> _expectedSatellites{};

};

TEST_F(TLESatcatDataReaderTest, getSatelliteCollectionEverythingIn) {
    //The TLE file is not well-formed, but good enough for this test case
    TLESatcatDataReader tleSatcatDataReader{"resources/testShrinkedSatcat.csv", "resources/test-tle4.txt"};

    auto actualSatellites = tleSatcatDataReader.getSatelliteCollection();

    ASSERT_EQ(actualSatellites.size(), 4);

    auto expectedSatellites = _expectedSatellites.begin();
    for(auto &sat : actualSatellites) {
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

TEST_F(TLESatcatDataReaderTest, getSatelliteCollectionOneMissingInTLE) {
    _expectedSatellites.erase(std::remove_if(_expectedSatellites.begin(), _expectedSatellites.end(),
                                             [](Satellite &sat) {return sat.getId() == 3;}));
    //The TLE file is not well-formed, but good enough for this test case
    TLESatcatDataReader tleSatcatDataReader{"resources/testShrinkedSatcat.csv", "resources/test-tle5.txt"};

    auto actualSatellites = tleSatcatDataReader.getSatelliteCollection();

    ASSERT_EQ(actualSatellites.size(), 3);

    auto expectedSatellites = _expectedSatellites.begin();
    for(auto &sat : actualSatellites) {
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

TEST_F(TLESatcatDataReaderTest, getSatelliteCollectionOneInSatCat) {
    _expectedSatellites.erase(_expectedSatellites.begin()+1, _expectedSatellites.end());
    //The TLE file is not well-formed, but good enough for this test case
    TLESatcatDataReader tleSatcatDataReader{"resources/testShrinkedSatcat2.csv", "resources/test-tle4.txt"};

    auto actualSatellites = tleSatcatDataReader.getSatelliteCollection();

    ASSERT_EQ(actualSatellites.size(), 1);

    auto expectedSatellites = _expectedSatellites.begin();
    for(auto &sat : actualSatellites) {
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

TEST_F(TLESatcatDataReaderTest, getSatelliteCollectionNoPartner) {
    _expectedSatellites.clear();
    //The TLE file is not well-formed, but good enough for this test case; Technically it is not empty, but it contains
    //a satellite with no partner in the satcat
    TLESatcatDataReader tleSatcatDataReader{"resources/testShrinkedSatcat2.csv", "resources/test-tle6.txt"};

    auto actualSatellites = tleSatcatDataReader.getSatelliteCollection();

    ASSERT_EQ(actualSatellites.size(), 0);
}

TEST_F(TLESatcatDataReaderTest, getSatelliteCollectionEmptyTLE) {
    _expectedSatellites.clear();
    TLESatcatDataReader tleSatcatDataReader{"resources/testShrinkedSatcat.csv", "resources/test-tle7.txt"};

    auto actualSatellites = tleSatcatDataReader.getSatelliteCollection();

    ASSERT_EQ(actualSatellites.size(), 0);
}
