#include "gtest/gtest.h"

#include <vector>
#include <string>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <exception>
#include <iostream>
#include "output/CSVWriter.h"
#include "model/Satellite.h"
#include "input/CSVReader.h"

class CSVWriterTest : public ::testing::Test {

protected:

    virtual void SetUp() {
        double area = 1.0;
        double mass = 100;
        double id = 0;
        _satelliteCollection.resize(static_cast<size_t>(4), Satellite("DebrisTestFragment", SatType::DEBRIS));
        for (auto &sat : _satelliteCollection) {
            sat.setId(++id);
            sat.setCharacteristicLength(0.25);
            sat.setArea(area);
            sat.setMass(mass);
            sat.setAreaToMassRatio(1.0 / 100.0);
            sat.setVelocity({1.0, 2.0, 3.0});
            area += 1.0;
            mass += 100.0;
        }

    }

    virtual void TearDown() {
        _satelliteCollection.clear();
        try {
            std::filesystem::remove(_filePath);
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    const std::string _filePath{"resources/csvTestFile.csv"};

    std::vector<Satellite> _satelliteCollection{};

};

TEST_F(CSVWriterTest, FileCreation) {
    CSVWriter csvWriter{_filePath};

    ASSERT_FALSE(std::filesystem::exists(_filePath)) << "File should not yet exist!";

    csvWriter.printResult(_satelliteCollection);

    ASSERT_TRUE(std::filesystem::exists(_filePath)) << "A CSV file should have been generated!";
}

TEST_F(CSVWriterTest, HeaderCheck) {
    CSVWriter csvWriter{_filePath};

    csvWriter.printResult(_satelliteCollection);

    CSVReader csvReader{_filePath, true};

    auto header = csvReader.getHeader();

    ASSERT_EQ(header.at(0), "ID");
    ASSERT_EQ(header.at(1), "Name");
    ASSERT_EQ(header.at(2), "Satellite Type");
    ASSERT_EQ(header.at(3), "Characteristic Length [m]");
    ASSERT_EQ(header.at(4), "A/M [m^2/kg]");
    ASSERT_EQ(header.at(5), "Area [m^2]");
    ASSERT_EQ(header.at(6), "Mass [kg]");
    ASSERT_EQ(header.at(7), "Velocity [m/s]");
    ASSERT_EQ(header.at(8), "Position [m]");

}

TEST_F(CSVWriterTest, DataCheck) {
    CSVWriter csvWriter{_filePath};

    csvWriter.printResult(_satelliteCollection);

    CSVReader csvReader{_filePath, true};

    size_t expectedID = 1;
    const std::string expectedName{"DebrisTestFragment"};
    const std::string expectedType{"DEBRIS"};
    double expectedCharacteristicLength = 0.25;
    double expectedArea = 1.0;
    double expectedMass = 100.0;
    double expectedAM = 1.0 / 100.0;
    const std::string expectedVelocity{"{1; 2; 3}"};
    const std::string expectedPosition{"{0; 0; 0}"};

    auto lines = csvReader.getLines();
    auto line = lines.begin();
    for (auto &sat : _satelliteCollection) {
        ASSERT_EQ(line->at(0), std::to_string(expectedID));
        ASSERT_EQ(line->at(1), expectedName);
        ASSERT_EQ(line->at(2), expectedType);
        ASSERT_DOUBLE_EQ(std::stod(line->at(3)), expectedCharacteristicLength);
        ASSERT_DOUBLE_EQ(std::stod(line->at(4)), expectedAM);
        ASSERT_DOUBLE_EQ(std::stod(line->at(5)), expectedArea);
        ASSERT_DOUBLE_EQ(std::stod(line->at(6)), expectedMass);
        ASSERT_EQ(line->at(7), expectedVelocity);
        ASSERT_EQ(line->at(8), expectedPosition);
        expectedID += 1;
        expectedArea += 1.0;
        expectedMass += 100.0;

        ++line;
    }
}
