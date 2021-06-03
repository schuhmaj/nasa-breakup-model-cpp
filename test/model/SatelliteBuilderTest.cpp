#include "gtest/gtest.h"

#include "model/SatelliteBuilder.h"
#include <array>


class SatelliteBuilderTest : public ::testing::Test {

protected:

    virtual void SetUp() {
        _satelliteBuilder = SatelliteBuilder{};
        _array123 = std::array<double, 3> {1.0,2.0,3.0};
        _array123 = std::array<double, 3> {0.0,0.0,0.0};
    }

    SatelliteBuilder _satelliteBuilder;
    std::array<double, 3> _array123;
    std::array<double, 3> _array000;

};

TEST_F(SatelliteBuilderTest, example1) {
    Satellite sputnik = _satelliteBuilder
            .setID(1)
            .setName("Sputnik 1")
            .setSatType("SPACECRAFT")
            .setMass(83)
            .setVelocity(_array123)
            .getResult();

    EXPECT_EQ(sputnik.getId(), 1);
    EXPECT_EQ(sputnik.getName(), "Sputnik 1");
    EXPECT_EQ(sputnik.getSatType(), SPACECRAFT);
    EXPECT_EQ(sputnik.getMass(), 83);
    EXPECT_EQ(sputnik.getVelocity(), _array123);
    EXPECT_EQ(sputnik.getPosition(), _array000);
}