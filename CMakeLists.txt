cmake_minimum_required(VERSION 3.5)
project(naivebayes)

set(CMAKE_CXX_STANDARD 14)

add_executable(naivebayes src/ImageData.cpp src/ImageData.h src/Probability.cpp src/Classfication.cpp src/Evaluation.cpp)
add_executable(unit_test tests/test.cpp src/ImageData.h tests/testsFunctions.cpp)