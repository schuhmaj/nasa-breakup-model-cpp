#Includes the gtest.cmake
include(gtest)

#Recursively collects all the test files in the test Directory
file(GLOB_RECURSE TEST_SRC
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*/*.h"
        )

#Creates the Test Target breakupModelTest
add_executable(breakupModelTest ${TEST_SRC})

#Adds the the source files from the src Directory to the include path
target_include_directories(breakupModelTest PRIVATE "${PROJECT_SOURCE_DIR}/src")

#Links target breakupModelTest against gtest_main (entry point, so no own main method is required)
target_link_libraries(breakupModelTest
        gtest_main
        yaml-cpp
        )

#Copies the test resources into a directory next to the test executable
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

include(GoogleTest)

# Adds the Tests to CTest by querying the test target executable
gtest_discover_tests(breakupModelTest)