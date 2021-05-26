include(FetchContent)

#Adapted from https://cliutils.gitlab.io/modern-cmake/chapters/testing/googletest.html
#Fetches the version 1.10.0 from the official github for googletest
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.10.0
)

FetchContent_GetProperties(googletest)

if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

#Recursively searches for all cpp files in the test folder
file(GLOB_RECURSE TEST_SRC
        "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp"
        )

#Enables CTest
enable_testing()

#Creates the Test Target
add_executable(breakupModelTest ${TEST_SRC})

#Links against gtest
target_link_libraries(breakupModelTest
        gtest_main
)

include(GoogleTest)

gtest_discover_tests(breakupModelTest)