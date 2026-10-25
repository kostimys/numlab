cmake_minimum_required(VERSION 3.10)
project(Lab2_BVP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Release by default (only for single-config generators: Ninja, Makefiles).
# For Visual Studio generator the configuration is chosen at build time (--config).
get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NOT IS_MULTI_CONFIG AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(LAB2_SOURCES
  main.cpp
  Problem.cpp
  Quadrature.cpp
  Progonka.cpp
  DifferenceScheme.cpp
  AnalyticSolution.cpp
  Experiments.cpp
  Report.cpp
  ConsoleUtf8.cpp)

# Clear message if some files were not copied next to CMakeLists.txt
foreach(src IN LISTS LAB2_SOURCES)
  if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
    message(FATAL_ERROR "Source file not found: ${src} (all .cpp and .h files must be in the same folder as CMakeLists.txt)")
  endif()
endforeach()

add_executable(lab2 ${LAB2_SOURCES})

if(MSVC)
  target_compile_options(lab2 PRIVATE /utf-8 /W3)   # sources are UTF-8 (Russian comments)
else()
  target_compile_options(lab2 PRIVATE -Wall -Wextra)
endif()

# Visual Studio generator: start-up project and working directory
# (CSV files will appear next to the sources)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT lab2)
set_target_properties(lab2 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
