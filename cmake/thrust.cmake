include(FetchContent)

FetchContent_Declare(thrust
        GIT_REPOSITORY https://github.com/NVIDIA/thrust.git
        GIT_TAG 1.16.0
        )

FetchContent_MakeAvailable(thrust)