# nasa-breakup-model-cpp
Implementation as part of the Bachelor Thesis "Efficient Implementation and Evaluation of the NASA Breakup Model in modern C++"

### Additional Information (TODO: Remove when public)
Additional Documents and Descriptions to the Implementation
and its fundamentals can be found here:
https://1drv.ms/u/s!Ag_epLxb0vqqgc53PKGgT8J1CdzrtQ?e=U2JT9E

### Requirements
The project uses the following dependencies:
- GoogleTest (Only required for Testing, Automatically set-up by CMake)
- yaml-cpp (Required for Input, Automatically set-up by CMake)

### Build
The program is build by using CMake. So first make sure that you installed
CMake and then follow these steps:

    mkdir build
    cd build
    cmake ..
    make

### Execution
After the build, the simulation can be run by executing:

    ./breakupModel [yaml-file]
    
### Testing
The tests use the framework GoogleTest and
can simply be run by executing in the build directory:

    ctest