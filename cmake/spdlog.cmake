include(FetchContent)

#Fetches the version 1.8.5 for spdlog
FetchContent_Declare(spdlog
        URL https://github.com/gabime/spdlog/archive/refs/tags/v1.8.5.zip
        )

FetchContent_MakeAvailable(spdlog)