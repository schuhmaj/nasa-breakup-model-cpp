include(FetchContent)

#Adapted from https://cliutils.gitlab.io/modern-cmake/chapters/testing/googletest.html
#Fetches the version 1.10.0 from the official github for googletest
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.10.0
        )

FetchContent_GetProperties(googletest)

if (NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif ()