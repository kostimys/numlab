cmake_minimum_required(VERSION 3.10)
project(Lab2_BVP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.10)
project(Lab2_Mixed)
set(CMAKE_CXX_STANDARD 17)
add_executable(myClass main.cpp Problem.cpp DifferenceScheme.cpp Quadrature.cpp Progonka.cpp AnalyticSolution.cpp)