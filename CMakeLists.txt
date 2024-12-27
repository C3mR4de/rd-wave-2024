cmake_minimum_required(VERSION 3.28)
project(rd-wave-2024 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(${PROJECT_NAME} src/main.cpp src/TickCounter.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics atomic)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

