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
- GoogleTest-1.10.0 (Only required for Testing, Automatically set-up by CMake)
- yaml-cpp-0.6.3 (Required for Input, Automatically set-up by CMake)
- spdlog Version 1.8.5 (Required for output and logging, Automatically set-up by CMake)

## Build
The program is build by using CMake. So first make sure that you installed
CMake and then follow these steps:

    mkdir build
    cd build
    cmake ..
    make

## Execution
After the build, the simulation can be run by executing:

    ./breakupModel [yaml-file]

### Input

The given yaml-file should look like this:

```yaml
  simulation:
    minimalCharacteristicLength: 0.05 #minimal fragment L_c in [m]
    simulationType: COLLISION         #Option (Alias): COLLISION (CO) or EXPLOSION (EX)
                                      #If not given type is determined
                                      # by number of input satellites 
    currentMaxID: 48514               #For determining fragment ID
                                      #Should be the currently largest given NORAD-Catalog ID
                                      #If not given, zero is assumed
    inputSource: ["../data.yaml"]     #Path to input file(s) - One of the following:
                                      #1) ["data.yaml"]
                                      #2) ["satcat.csv", "tle.txt"]
    idFilter: [1, 2]                  #Only the satellites with these IDs will be
                                      #recognized by the simulation.
                                      #If not given, no filter is applied
  inputOutput:                        #If you want to print out the input data into specific file (optional)
    target: ["input.csv", "input.vtu"]#Target files
    #kepler: True                     #CSV with Kepler elements
    #csvPattern: "IL"                 #Override "kepler" setting and prints CSV output according to pattern
  resultOutput:                       #If you want a result, you should define here some target file for the
    target: ["result.csv", "result.vtu"]#fragements (like vtk or csv)
    #kepler: True                     #Option like above
    #csvPattern: "IL"                 #Option like above, available Patterns: see below
```    
A "data.yaml" should have the following form (for example):

```yaml
    satellites:
    - name: "Example Satellite 1"   #Optional
      id: 24946                     #A must e. g. NORAD Catalog ID)
      satType: SPACECRAFT           #Optional (Default: SPACECRAFT); Option (Alias):
                                    #SPACECRAFT (SC), ROCKET_BODY (RB), DEBRIS (DEB), UNKNOWN
      mass: 700.0                   #Either mass or area is a must
                                    #Mass in [kg], Area (Radar Cross Section) in [m^2]
      velocity: [1.0, 1.0, 1.0]     #The cartesian velcoity vector [m/s]
      position: [0.0, 0.0, 0.0]     #The cartesian position [m] (Optional)
    - name: "Example Satellite 2"   #Second example, again name is optional
      id: 24947                     #A must
      satType: ROCKET_BODY          #Another satellite type
      area: 3.5                     #Either mass or area is a must
                                    #Mass in [kg], Area (Radar Cross Section) in [m^2]
      kepler:                       #Either velocity (+ position) or
        semi-major-axis: 6798505.86 #Keplerian Elements
        eccentricity: 0.0002215     #Angles should be given in [rad]
        inclination: 0.9013735469   #semi-major-axis in [m]
        longitude-of-the-ascending-node: 4.724103630312
        argument-of-periapsis: 2.237100203348
        true-anomaly: (double)      #You don't have to give all three of them to the simulation
        eccentric-anomaly: (double) #But one of them is a must.
        mean-anomaly: 0.2405604761  #The program ignores if multipe anomalies are given
                                    #expect one.The precedence is: Eccentric > Mean > True
    - name: "Example Satellite 3"   
      id: 24948                     
      satType: SPACECRAFT           
      mass: 700.0                   
      kepler: "../tle-file.txt"     #It is also possible to parse the Kepler elements from
                                    #a TLE file
    - ...
```

A "satcat.csv" has the following form:

    OBJECT_NAME,OBJECT_ID,NORAD_CAT_ID,OBJECT_TYPE,OPS_STATUS_CODE,OWNER,LAUNCH_DATE,LAUNCH_SITE,DECAY_DATE,PERIOD,INCLINATION,APOGEE,PERIGEE,RCS,DATA_STATUS_CODE,ORBIT_CENTER,ORBIT_TYPE
    SL-1 R/B,1957-001A,1,R/B,D,CIS,1957-10-04,TYMSC,1957-12-01,96.19,65.10,938,214,20.4200,,EA,IMP
    SPUTNIK 1,1957-001B,2,PAY,D,CIS,1957-10-04,TYMSC,1958-01-03,96.10,65.00,1080,64,,,EA,IMP
    SPUTNIK 2,1957-002A,3,PAY,D,CIS,1957-11-03,TYMSC,1958-04-14,103.74,65.33,1659,211,0.0800,,EA,IMP
    EXPLORER 1,1958-001A,4,PAY,D,US,1958-02-01,AFETR,1970-03-31,88.48,33.15,215,183,,,EA,IMP
    ...

It can be obtained from the following source:
https://celestrak.com/pub/satcat.csv

A "tle.txt" has the following form:

    ISS (ZARYA)
    1 25544U 98067A   08264.51782528 -.00002182  00000-0 -11606-4 0  2927
    2 25544  51.6416 247.4627 0006703 130.5360 325.0288 15.72125391563537
    ...

Example taken from https://en.wikipedia.org/wiki/Two-line_element_set.
The format specification can also be read there.

Notice that if you decide to take the satcat + TLE approach that the satellite(s)
which should explode (collide) must be present in both files. The mapping is done
via their IDs. You find the IDs in the satcat in the third column (NORAD_CAT_ID) and
in the TLE optionally in first and second row in the beginning (example: 25544).

### Output
The simulation can produce depending on the configuration CSV and VTK files.

#### Available Patterns for the CSVPatternWriter:

| Pattern | Meaning                   | Pattern | Meaning |
| ---     | ---                       | ---     | --- |
| I       | ID                        | a       | Semi-Major-Axis [m] |
| n       | Name                      | e       | Eccentricity |
| t       | Satellite Type            | i       | Inclination [rad] |
| L       | Characteristic Length [m] | W       | Longitude of the ascending node [rad] |
| R       | A/M [m^2/kg]              | w       | Argument of periapsis [rad] |
| A       | Area [m^2]                | M       | Mean Anomaly [rad] |
| m       | Mass [kg]                 | E       | Eccentric Anomaly [rad] |
| v       | Velocity [m/s]            | T       | True Anomaly [rad] |
| p       | Position [m]              |         | |

## Library
The Breakup Simulation can be used directly from any C++ project
without the need of file input, etc.
To create a new Breakup Simulation just follow these four steps:
    
1. Create a new RuntimeInputSource and define the parameter
   (use the SatelliteBuilder to easier create new Satellites
   and get feedback about the completeness)
2. Give this RuntimeInputSource to the BreakupBuilder
3. Create your Breakup Simulation
4. Run and get the Result!

Example:

```cpp
    //RuntimeInput via the RuntimeInputSource Object (minmial Config: minL_c = 0.05 + inputSatellites)
    auto configSource = std::make_shared<RuntimeInputSource>(0.05, satellites);
    //Give the BreakupBuilder its configuration object (YMALConfigurationReader or RuntimeInputSource or your own derived source)
    BreakupBuilder breakupBuilder{configSource};
    //Create the Breakup Simulation
    auto breakup = breakupBuilder.getBreakup();
    //Run it and get the result via breakup.getResult();
    breakup->run();
```
## Testing
The tests use the framework GoogleTest and
can simply be run by executing in the build directory:

    ctest
