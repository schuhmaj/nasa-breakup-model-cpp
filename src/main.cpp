#include <iostream>
#include <string>
#include <memory>
#include <exception>

#include "input/YAMLDataReader.h"
#include "input/YAMLConfigurationReader.h"
#include "simulation/BreakupFactory.h"
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

    //Load an Configuration Reader which contains the necessary config + data for the BreakupFactory
    auto inputSource = std::shared_ptr<ConfigurationReader>{new YAMLConfigurationReader{fileName}};

    //TODO Remove this step
    auto inputData = inputSource->getDataReader();

    //The SimulationFactory which builds our breakup simulation
    BreakupFactory breakupFactory{inputData};

    //Create and run the simulation or catch an exception in case something is wrong with the simulation
    try {
        //Creates and Runs the simulation
        auto breakUpSimulation = breakupFactory.getBreakupTypeByInput();
        breakUpSimulation->run();

        //Prints the the output to a CSV file
        auto output = std::unique_ptr<OutputWriter>{new CSVWriter{"result.csv"}};
        output->printResult(*breakUpSimulation);
    } catch (std::exception &e) {
        std::cerr << e.what();
    }

    return 0;
}
