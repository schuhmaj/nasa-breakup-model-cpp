#include "gtest/gtest.h"

#include <string>
#include <array>
#include <tuple>
#include "input/CSVReader.h"
#include "model/Satellite.h"

class CSVReaderTest : public ::testing::Test {


};

TEST_F(CSVReaderTest, readHeader) {
    std::array<std::string, 17> expectedHeader{"OBJECT_NAME", "OBJECT_ID", "NORAD_CAT_ID", "OBJECT_TYPE",
                                               "OPS_STATUS_CODE",
                                               "OWNER", "LAUNCH_DATE", "LAUNCH_SITE", "DECAY_DATE", "PERIOD",
                                               "INCLINATION", "APOGEE", "PERIGEE", "RCS", "DATA_STATUS_CODE",
                                               "ORBIT_CENTER", "ORBIT_TYPE"};

    CSVReader<std::string, std::string, size_t,
            SatType, std::string, std::string, std::string, std::string, std::string,
            double, double, double, double, double,
            std::string, std::string, std::string> satcatReader{"resources/testShrinkedSatcat.csv", true};

    auto actualHeader = satcatReader.getHeader();

    EXPECT_EQ(actualHeader, expectedHeader);
}

TEST_F(CSVReaderTest, readLines) {
    auto expectedTuple01 = std::make_tuple("SL-1 R/B", "1957-001A", 1, SatType::ROCKET_BODY, "D", "CIS", "1957-10-04", "TYMSC",
                                   "1957-12-01", 96.19, 65.10, 938.0, 214.0, 20.4200, "", "EA", "IMP");
    auto expectedTuple02 = std::make_tuple("SPUTNIK 1", "1957-001B", 2, SatType::SPACECRAFT, "D", "CIS", "1957-10-04", "TYMSC",
                                   "1958-01-03", 96.10, 65.00, 1080.0, 64.0, 0.0, "", "EA", "IMP");
    auto expectedTuple03 = std::make_tuple("SPUTNIK 2", "1957-002A", 3, SatType::SPACECRAFT, "D", "CIS", "1957-11-03", "TYMSC",
                                   "1958-04-14", 103.74, 65.33, 1659.0, 211.0, 0.0800, "", "EA", "IMP");
    auto expectedTuple04 = std::make_tuple("EXPLORER 1", "1958-001A", 4, SatType::SPACECRAFT, "D", "US", "1958-02-01", "AFETR",
                                   "1970-03-31", 88.48, 33.15, 215.0, 183.0, 0.0, "", "EA", "IMP");

    CSVReader<std::string, std::string, size_t,
            SatType, std::string, std::string, std::string, std::string, std::string,
            double, double, double, double, double,
            std::string, std::string, std::string> satcatReader{"resources/testShrinkedSatcat.csv", true};

    auto actualTuple = satcatReader.getLines();

    ASSERT_EQ(actualTuple[0], expectedTuple01);
    ASSERT_EQ(actualTuple[1], expectedTuple02);
    ASSERT_EQ(actualTuple[2], expectedTuple03);
    ASSERT_EQ(actualTuple[3], expectedTuple04);
}