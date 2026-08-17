cmake_minimum_required(VERSION 3.15)
project(aura_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(pybind11 REQUIRED)

pybind11_add_module(aura_engine
    engine/src/bindings.cpp
    engine/src/pipeline.cpp
    engine/src/scanner.cpp
    engine/src/phash.cpp
    engine/src/matcher.cpp
    engine/src/vptree.cpp
)

target_include_directories(aura_engine PRIVATE engine/src engine/external)

# Drop the built module (aura_engine*.pyd / .so) straight into the repo
# root so it's importable with no manual copying once Python code shows up.
set_target_properties(aura_engine PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)