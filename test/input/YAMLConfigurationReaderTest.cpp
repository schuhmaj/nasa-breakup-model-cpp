#include "gtest/gtest.h"

#include <string>
#include <set>
#include "input/YAMLConfigurationReader.h"
#include "model/Satellite.h"

TEST(YAMLConfigurationReaderTest, ConfigTest01_Normal) {
    std::set<size_t> expectedIDFilter = {123, 456};

    YAMLConfigurationReader yamlReader{"resources/YamlConfigurationReaderTest01.yaml"};

    ASSERT_EQ(yamlReader.getMinimalCharacteristicLength(), 0.05);
    ASSERT_EQ(yamlReader.getTypeOfSimulation(), SimulationType::COLLISION);
    ASSERT_EQ(yamlReader.getCurrentMaximalGivenID().value(), 48514);
    ASSERT_EQ(yamlReader.getIDFilter().value(), expectedIDFilter);


    ASSERT_EQ(yamlReader.getInputTargets().size(), 1);
    ASSERT_EQ(yamlReader.getOutputTargets().size(), 2);
}

TEST(YAMLConfigurationReaderTest, ConfigTest02_Normal) {
    std::set<size_t> expectedIDFilter = {789, 101112, 131415, 1617181920};

    YAMLConfigurationReader yamlReader{"resources/YamlConfigurationReaderTest02.yaml"};

    ASSERT_EQ(yamlReader.getMinimalCharacteristicLength(), 0.10);
    ASSERT_EQ(yamlReader.getTypeOfSimulation(), SimulationType::COLLISION);
    ASSERT_EQ(yamlReader.getCurrentMaximalGivenID(), std::nullopt);
    ASSERT_EQ(yamlReader.getIDFilter().value(), expectedIDFilter);


    ASSERT_EQ(yamlReader.getInputTargets().size(), 0);
    ASSERT_EQ(yamlReader.getOutputTargets().size(), 0);
}

TEST(YAMLConfigurationReaderTest, ConfigTest03_NoException) {
    ASSERT_NO_THROW(YAMLConfigurationReader {"resources/YamlConfigurationReaderTest02.yaml"})
    << "No output specs are not fatal, so this should not throw an exception";
}

TEST(YAMLConfigurationReaderTest, ConfigTest04_MinimalConfig) {
    YAMLConfigurationReader yamlReader{"resources/YamlConfigurationReaderTest03.yaml"};

    ASSERT_EQ(yamlReader.getMinimalCharacteristicLength(), 0.10);
    ASSERT_EQ(yamlReader.getTypeOfSimulation(), SimulationType::UNKNOWN);
    ASSERT_EQ(yamlReader.getCurrentMaximalGivenID(), std::nullopt);
    ASSERT_EQ(yamlReader.getIDFilter(), std::nullopt);


    ASSERT_EQ(yamlReader.getInputTargets().size(), 0);
    ASSERT_EQ(yamlReader.getOutputTargets().size(), 0);
}

TEST(YAMLConfigurationReaderTest, ConfigTest05_NoSimTag) {
    ASSERT_THROW(YAMLConfigurationReader {"resources/YamlConfigurationReaderTest04.yaml"},
                 std::runtime_error) << "The file does not contain a simulation tag, this should throw "
                                        "an exception";
}

