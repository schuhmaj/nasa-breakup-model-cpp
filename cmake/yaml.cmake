include(FetchContent)

#Fetches the version 0.6.3 for yaml-cpp
FetchContent_Declare(yaml
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG yaml-cpp-0.6.3
        )

FetchContent_MakeAvailable(yaml)