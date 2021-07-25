#include "gtest/gtest.h"

#include <vector>
#include <string>
#include <filesystem>
#include "output/VTKWriter.h"
#include "model/Satellite.h"

class VTKWriterTest : public ::testing::Test {

protected:

    virtual void SetUp() {
        double area = 1.0;
        double mass = 100;
        size_t id = 0;
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

    const std::string _filePath{"resources/vtkTestFile.vtu"};

    std::vector<Satellite> _satelliteCollection{};

};

TEST_F(VTKWriterTest, FileCreation) {
    ASSERT_FALSE(std::filesystem::exists(_filePath)) << "File should not yet exist!";

    auto vtkTestLogger = spdlog::basic_logger_mt("VTKWriterTest", _filePath, true);
    VTKWriter vtkWriter{vtkTestLogger};

    ASSERT_TRUE(std::filesystem::exists(_filePath)) << "A CSV file should have been generated!";
}