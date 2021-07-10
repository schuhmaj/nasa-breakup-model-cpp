#include <iostream>
#include <string>
#include <memory>
#include <exception>

#include "input/YAMLDataReader.h"
#include "input/YAMLConfigurationReader.h"
#include "simulation/BreakupBuilder.h"
#include "output/CSVWriter.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cout <<
            "Wrong program call. Please call the program in the following way:\n" <<
            "./breakupModel [yaml-file]" <<
            std::endl;
        return 0;
    }

    //The fileName of the YAML file
    std::string fileName{argv[1]};

    //Load an Configuration Reader which contains the necessary config + data for the BreakupBuilder
    auto configurationSource = std::shared_ptr<ConfigurationSource>{new YAMLConfigurationReader{fileName}};

    //The SimulationFactory which builds our breakup simulation
    BreakupBuilder breakupBuilder{configurationSource};

    //Create and run the simulation or catch an exception in case something is wrong with the simulation
    try {
        //Creates and Runs the simulation
        auto breakUpSimulation = breakupBuilder.getBreakup();
        breakUpSimulation->run();

        //Prints the the output to a CSV file
        auto output = std::unique_ptr<OutputWriter>{new CSVWriter{"result.csv", configurationSource->getOutputWithKepler()}};
        output->printResult(*breakUpSimulation);
    } catch (std::exception &e) {
        std::cerr << e.what();
    }

    return 0;
}
