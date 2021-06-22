#include "gtest/gtest.h"

#include <vector>
#include <algorithm>
#include <cmath>
#include <memory>
#include "model/Satellite.h"
#include "model/SatelliteBuilder.h"
#include "simulation/Collision.h"

class CollisionTest : public ::testing::Test {

protected:

    virtual void SetUp() {
        SatelliteBuilder satelliteBuilder{};
        sat1 = satelliteBuilder
                .setID(24946)
                .setName("Iridium 33")
                .setSatType(SatType::SPACECRAFT)
                .setMass(560)
                .setVelocity({11700.0, 0.0, 0.0})
                .getResult();
        sat2 = satelliteBuilder
                .setID(22675)
                .setName("Kosmos 2251")
                .setSatType(SatType::SPACECRAFT)
                .setMass(950)
                .setVelocity({0, 0.0, 0.0})
                .getResult();

        _input.push_back(sat1);
        _input.push_back(sat2);


        _collision = std::make_unique<Collision>(_input, _minimalCharacteristicLength);
        _collision->run();
        _output = _collision->getResult();

    }

    std::vector<Satellite> _input{};
    std::vector<Satellite> _output{};

    Satellite sat1;
    Satellite sat2;

    double _minimalCharacteristicLength{0.05};

    std::unique_ptr<Collision> _collision;

};


TEST_F(CollisionTest, FragmentCountTest) {
    ASSERT_TRUE(_collision->isIsCatastrophic()) << "This collision was catastrophic";

    size_t expectedFragmentCount = 4064; //M=m_1+m_2 & L_c = 0.05 --> Equation 4

    ASSERT_EQ(_output.size(), expectedFragmentCount);
}


TEST_F(CollisionTest, FragmentSizeDsitributionTest) {
    double Lc1 = sat1.getCharacteristicLength();
    double Lc2 = sat2.getCharacteristicLength();
    double expectedMaximalCharacteristicLength = Lc1 > Lc2 ? Lc1 : Lc2;

    ASSERT_FLOAT_EQ(_collision->getMaximalCharacteristicLength(), expectedMaximalCharacteristicLength);

    double Lc = _minimalCharacteristicLength;

    //1% Deviation for the Test Case (--> +-40 Fragments)
    double deviation = static_cast<double>(_output.size()) * 0.01;

    while(Lc < expectedMaximalCharacteristicLength) {
        size_t count = std::count_if(_output.begin(), _output.end(),[Lc](Satellite &sat) {
            return sat.getCharacteristicLength() > Lc;
        });

        double expectedCount = 0.1 * std::pow(sat1.getMass() + sat2.getMass(), 0.75) * std::pow(Lc, -1.71);

        size_t expectedUpperBound = static_cast<size_t>(expectedCount + deviation);
        size_t expectedLowerBound = expectedCount - deviation > 0 ? static_cast<size_t>(expectedCount - deviation) : 0;

        ASSERT_GE(count, expectedLowerBound);
        ASSERT_LE(count, expectedUpperBound);

        Lc += 0.1;
    }

}

TEST_F(CollisionTest, AMDistributionTest) {


}

TEST_F(CollisionTest, DeltaVelocityDistributionTest) {


}