#include "gtest/gtest.h"

#include <string>
#include <vector>
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
    CSVReader<std::string, std::string, size_t,
            SatType, std::string, std::string, std::string, std::string, std::string,
            double, double, double, double, double,
            std::string, std::string, std::string> satcatReader{"resources/testShrinkedSatcat.csv", true};

    auto lines = satcatReader.getLines();


}
