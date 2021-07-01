#include <iostream>
#include <string>
#include <memory>
#include <exception>

#include "input/YAMLReader.h"
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

    //The InputReader
    auto inputSource = std::shared_ptr<InputReader>{new YAMLReader{fileName}};

    //The SimulationFactory which builds our breakup simulation
    BreakupFactory breakupFactory{inputSource};

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
