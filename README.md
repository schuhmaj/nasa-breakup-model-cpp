# nasa-breakup-model-cpp
Implementation as part of the Bachelor Thesis "Efficient Implementation and Evaluation of the NASA Breakup Model in modern C++"

Comments to Equations or similar in the source code reference the following document:

Johnson, Nicholas L., et al. “NASA’s new breakup model of EVOLVE 4.0” Advances in Space Research 28.9 (2001): 1377-1384

### Additional Information (TODO: Remove when public)
Additional Documents and Descriptions to the Implementation
and its fundamentals can be found here:
https://1drv.ms/u/s!Ag_epLxb0vqqgc53PKGgT8J1CdzrtQ?e=U2JT9E

### Requirements
The project uses the following dependencies:
- GoogleTest-1.10.0 (Only required for Testing, Automatically set-up by CMake)
- yaml-cpp-0.6.3 (Required for Input, Automatically set-up by CMake)

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

    minimalCharacteristicLength: 0.05 #minimal fragment L_c in [m]
    simulationType: COLLISION         #Option (Alias): COLLISION (CO) or EXPLOSION (EX)
                                      #If not given type is determined
                                      # by number of input satellites 
    currentMaxID: 48514               #For determining fragment ID
                                      #Should be the currently largest given NORAD-Catalog ID
                                      #If not given, zero is assumed
    inputSource: ["../data.yaml"]     #Path to input file(s) - One of the following:
                                      #1) ["data.yaml"]
                                      #2) ["satcat.csv, "tle.txt"]
    
If the "data.yaml" should have the following form (for example):

    satellites:
    - name: "Example Satellite"     #Optional
      id: 24946                     #A must e. g. NORAD Catalog ID)
      satType: SPACECRAFT           #Optional (Default: SPACECRAFT); Option (Alias):
                                    #SPACECRAFT (SC), ROCKET_BODY (RB), DEBRIS (DEB), UNKNOWN
      mass: 700.0                   #Either mass or area is a must
      area: 3.5                     #Mass in [kg], Area (Radar Cross Section) in [m^2]
      velocity: [1.0, 1.0, 1.0]     #The cartesian velcoity vector [m/s]
    - ...

### Testing
The tests use the framework GoogleTest and
can simply be run by executing in the build directory:

    ctest