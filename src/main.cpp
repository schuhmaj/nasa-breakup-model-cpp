#include <iostream>
#include <string>
#include <memory>
#include <exception>

#include "input/YAMLDataReader.h"
#include "input/YAMLConfigurationReader.h"
#include "simulation/BreakupBuilder.h"
#include "output/CSVWriter.h"
#include "output/VTKWriter.h"
#include "spdlog/spdlog.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        spdlog::error(
                "Wrong program call. Please call the program in the following way:\n"
                "./breakupModel [yaml-file]");
        return 0;
    }
    try {
        //The fileName of the YAML file
        std::string fileName{argv[1]};

        //Load an Configuration Source containing the input arguments required for the BreakupBuilder
        //The YAMLConfigurationReader as a special case is also able to load the Configuration for Output
        auto configSource = std::make_shared<YAMLConfigurationReader>(fileName);

        //The SimulationFactory which builds our breakup simulation
        BreakupBuilder breakupBuilder{configSource};

        //Create and run the simulation or catch an exception in case something is wrong with the simulation
        //Creates and Runs the simulation
        auto breakUpSimulation = breakupBuilder.getBreakup();
        breakUpSimulation->run();

        //Prints the the output to files defined by the OutputConfigurationSource aka the YAMLConfigurationReader
        auto outputTargets = configSource->getOutputTargets();
        for (auto &out : outputTargets) {
            out->printResult(*breakUpSimulation);
        }
    } catch (std::exception &e) {
        spdlog::error(e.what());
    }
    return 0;
}
