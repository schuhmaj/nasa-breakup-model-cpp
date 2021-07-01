#include "gtest/gtest.h"

#include <vector>
#include <algorithm>
#include <cmath>
#include <memory>
#include "model/Satellite.h"
#include "model/SatelliteBuilder.h"
#include "simulation/Explosion.h"

class ExplosionTest : public ::testing::Test {

protected:

    virtual void SetUp() {
        SatelliteBuilder satelliteBuilder{};
        sat = satelliteBuilder
                .setID(7946)
                .setName("1975-052B")   //Nimbus 6 R/B breakup due to Propulsion system
                .setSatType(SatType::ROCKET_BODY)
                .setMass(839)           //Upper Stage was a Delta 2nd stage with 839 kg mass
                .setVelocity({0.0, 0.0, 0.0})   //Irrelevant
                .getResult();

        _input.push_back(sat);


        _explosion = std::make_unique<Explosion>(_input, _minimalCharacteristicLength);
        _explosion->run();
        _output = _explosion->getResult();

    }

    std::vector<Satellite> _input{};
    std::vector<Satellite> _output{};

    Satellite sat;

    double _minimalCharacteristicLength{0.05};

    std::unique_ptr<Explosion> _explosion;

};


TEST_F(ExplosionTest, FragmentCountTest) {
    size_t expectedFragmentCount = 724; //L_c = 0.05
    ASSERT_EQ(_output.size(), expectedFragmentCount);
}


TEST_F(ExplosionTest, FragmentSizeDsitributionTest) {
    double expectedMaximalCharacteristicLength = sat.getCharacteristicLength();

    ASSERT_FLOAT_EQ(_explosion->getMaximalCharacteristicLength(), expectedMaximalCharacteristicLength);

    double Lc = _minimalCharacteristicLength;

    //2% Deviation for the Test Case (--> +-14 Fragments)
    double deviation = static_cast<double>(_output.size()) * 0.02;

    while(Lc < expectedMaximalCharacteristicLength) {
        size_t count = std::count_if(_output.begin(), _output.end(),[Lc](Satellite &sat) {
            return sat.getCharacteristicLength() > Lc;
        });

        double expectedCount = 6 * std::pow(Lc, -1.6);

        size_t expectedUpperBound = static_cast<size_t>(expectedCount + deviation);
        size_t expectedLowerBound = expectedCount - deviation > 0 ? static_cast<size_t>(expectedCount - deviation) : 0;

        ASSERT_GE(count, expectedLowerBound);
        ASSERT_LE(count, expectedUpperBound);

        Lc += 0.1;
    }

}

TEST_F(ExplosionTest, AMDistributionTest) {

}

TEST_F(ExplosionTest, DeltaVelocityDistributionTest) {

}