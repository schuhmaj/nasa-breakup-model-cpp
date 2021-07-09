# nasa-breakup-model-cpp
![Build and Test](https://github.com/schuhmaj/nasa-breakup-model-cpp/actions/workflows/ctest.yml/badge.svg)

Implementation as part of the Bachelor Thesis "Efficient Implementation and Evaluation of the NASA Breakup Model in modern C++"

Comments to Equations or similar in the source code reference the following document:

Johnson, Nicholas L., et al. “NASA’s new breakup model of EVOLVE 4.0” Advances in Space Research 28.9 (2001): 1377-1384

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
    
The given yaml-file should look like this:

    satellites:
    - name: "Example Satellite"     #Optional
      id: 24946                     #A must e. g. NORAD Catalog ID)
      satType: SPACECRAFT           #Optional (Default: SPACECRAFT)
      mass: 700.0                   #Either mass or area is a must
      area: 3.5                     #Mass in [kg], Area (Radar Cross Section) in [m^2]
      velocity: [1.0, 1.0, 1.0]     #The cartesian velcoity vector [m/s]
    - ...

### Testing
The tests use the framework GoogleTest and
can simply be run by executing in the build directory:

    ctest
