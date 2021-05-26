cmake_minimum_required(VERSION 3.16)
project(breakupModel)

set(CMAKE_CXX_STANDARD 17)

#Collect the cpp files, if new files are added a Reload of the CMake Project is Required!
file(GLOB_RECURSE SRC
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        )

#Creates the executable target from the source files
add_executable(breakupModel ${SRC})

#Appends the the module path to contain additional CMake modules for this project
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

#Includes the gtest.cmake
include(gtest)