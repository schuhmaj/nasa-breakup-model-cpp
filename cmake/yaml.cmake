include(FetchContent)

#Fetches the version 0.6.3 for yaml-cpp
FetchContent_Declare(yaml
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG yaml-cpp-0.6.3
        )

FetchContent_MakeAvailable(yaml)

# Disable warnings from the library target
target_compile_options(yaml-cpp PRIVATE -w)
# Disable warnings from included headers
get_target_property(propval yaml-cpp INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(yaml-cpp SYSTEM PUBLIC "${propval}")
