#include <iostream>
#include <string>
#include <memory>

#include "input/YAMLReader.h"
#include "simulation/BreakupFactory.h"

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

    //Create and run the simulation
    //TODO Here it will always be a collision. See TODO in BreakupFactory
    auto breakUpSimulation = breakupFactory.getBreakupTypeByInput();
    breakUpSimulation->run();

    //TODO Establish interface to Output
    breakUpSimulation->getResult();

    return 0;
}
