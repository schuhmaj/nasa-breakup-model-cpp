#include "gtest/gtest.h"

#include "util/UtilityFunctions.h"


class UtilityFunctions : public ::testing::TestWithParam<double> {

};

TEST_P(UtilityFunctions, CharacteristicLengthFromMass){
    double expectedCharacteristicLength = GetParam();

    double mass = calculateSphereMass(expectedCharacteristicLength);

    double actualCharacteristicLength = calculateCharacteristicLengthFromMass(mass);

    ASSERT_NEAR(actualCharacteristicLength, expectedCharacteristicLength, 0.001);
}

INSTANTIATE_TEST_CASE_P(Tests, UtilityFunctions,
                        ::testing::Values(0.2, 0.7589, 2.42, 2.5, 3, 7.89));