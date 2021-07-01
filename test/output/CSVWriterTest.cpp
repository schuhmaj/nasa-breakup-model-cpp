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

class CSVWriterTest : public ::testing::Test {

protected:

    virtual void SetUp() {
        double area = 1.0;
        double mass = 100;
        _satelliteCollection.resize(static_cast<size_t>(4), Satellite("DebrisTestFragment", SatType::DEBRIS));
        for (auto &sat : _satelliteCollection) {
            sat.setId(++Satellite::currentMaxGivenID);
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
        Satellite::currentMaxGivenID = 0;
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

/**
 * Parses a Line from an CSV file istream and returns the cells stored in a vector.
 * @note Adapted from https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c [22.06.2021]
 * @param istream - the input stream
 * @return a vector containing the cells of the csv line
 */
std::vector<std::string> getNextCSVLine(std::istream &istream) {
    std::string line;
    std::getline(istream, line);
    std::stringstream lineStream{line};

    std::vector<std::string> result{};
    std::string cell;
    while(std::getline(lineStream, cell, ',')) {
        result.push_back(cell);
    }

    return result;
}

TEST_F(CSVWriterTest, FileCreation) {
    CSVWriter csvWriter{_filePath};

    ASSERT_FALSE(std::filesystem::exists(_filePath)) << "File should not yet exist!";

    csvWriter.printResult(_satelliteCollection);

    ASSERT_TRUE(std::filesystem::exists(_filePath)) << "A CSV file should have been generated!";
}

TEST_F(CSVWriterTest, HeaderCheck) {
    CSVWriter csvWriter{_filePath};

    csvWriter.printResult(_satelliteCollection);

    std::ifstream csvFile{_filePath};

    auto header = getNextCSVLine(csvFile);

    ASSERT_EQ(header.at(0), "ID");
    ASSERT_EQ(header.at(1), "Name");
    ASSERT_EQ(header.at(2), "Satellite Type");
    ASSERT_EQ(header.at(3), "Characteristic Length [m]");
    ASSERT_EQ(header.at(4), "A/M [m^2/kg]");
    ASSERT_EQ(header.at(5), "Area [m^2]");
    ASSERT_EQ(header.at(6), "Mass [kg]");
    ASSERT_EQ(header.at(7), "Velocity [m/s]");
    ASSERT_EQ(header.at(8), "Position [m]");

    csvFile.close();
}

TEST_F(CSVWriterTest, DataCheck) {
    CSVWriter csvWriter{_filePath};

    csvWriter.printResult(_satelliteCollection);

    std::ifstream csvFile{_filePath};

    //Skip Header
    getNextCSVLine(csvFile);

    size_t expectedID = 1;
    const std::string expectedName{"DebrisTestFragment"};
    const std::string expectedType{"DEBRIS"};
    double expectedCharacteristicLength = 0.25;
    double expectedArea = 1.0;
    double expectedMass = 100.0;
    double expectedAM = 1.0 / 100.0;
    const std::string expectedVelocity{"{1; 2; 3}"};
    const std::string expectedPosition{"{0; 0; 0}"};


    for (auto &sat : _satelliteCollection) {
        auto line = getNextCSVLine(csvFile);

        ASSERT_EQ(line.at(0), std::to_string(expectedID));
        ASSERT_EQ(line.at(1), expectedName);
        ASSERT_EQ(line.at(2), expectedType);
        ASSERT_DOUBLE_EQ(std::stod(line.at(3)), expectedCharacteristicLength);
        ASSERT_DOUBLE_EQ(std::stod(line.at(4)), expectedAM);
        ASSERT_DOUBLE_EQ(std::stod(line.at(5)), expectedArea);
        ASSERT_DOUBLE_EQ(std::stod(line.at(6)), expectedMass);
        ASSERT_EQ(line.at(7), expectedVelocity);
        ASSERT_EQ(line.at(8), expectedPosition);
        expectedID += 1;
        expectedArea += 1.0;
        expectedMass += 100.0;
    }

    csvFile.close();
}
