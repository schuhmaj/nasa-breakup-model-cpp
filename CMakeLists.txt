cmake_minimum_required(VERSION 3.16)
project(breakupModel)

set(CMAKE_CXX_STANDARD 17)

#Creates the executable target from the source files
add_executable(breakupModel ${PROJECT_SOURCE_DIR}/src/main.cpp)

#Adds the include Path PROJECT_SOURCE_DIR/src to the target breakupModel
target_include_directories(breakupModel PRIVATE "${PROJECT_SOURCE_DIR}/src")

#Appends the the module path to contain additional CMake modules for this project
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

#Enables CTest, must be in the top-level CMakeList.txt, otherwise it won't work
enable_testing()

#Subdirectory where the tests are located
add_subdirectory(${PROJECT_SOURCE_DIR}/test)