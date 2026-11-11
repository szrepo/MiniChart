cmake_minimum_required(VERSION 3.15)
project(MiniChart LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Raylib (downloaded automatically) ----
include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib)

# ---- Our program ----
add_executable(MiniChart src/main.cpp
        src/main.cpp
        src/main.cpp
        src/main.cpp
        include/Candle.h
        include/DataLoader.h
        src/DataLoader.cpp
)

target_include_directories(MiniChart PRIVATE include)
target_link_libraries(MiniChart PRIVATE raylib)

# So the code can find the data folder no matter where it runs from
target_compile_definitions(MiniChart PRIVATE
        DATA_DIR="${CMAKE_SOURCE_DIR}/data")