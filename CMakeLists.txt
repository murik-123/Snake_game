cmake_minimum_required(VERSION 3.28)
project(MyGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.1.0
        GIT_SHALLOW ON
)

FetchContent_MakeAvailable(SFML)

add_executable(MyGame main.cpp)
target_link_libraries(MyGame PRIVATE SFML::Graphics)