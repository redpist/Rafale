cmake_minimum_required(VERSION 2.8)

#while std=c++11 isn't default behaviour, you have to put the two following lines
SET(CMAKE_CXX_COMPILER g++-4.7)
SET(CMAKE_CXX_FLAGS "-Wextra -Wall -std=c++11 -O3")

#Find Rafale CMake module
FIND_PACKAGE(Rafale REQUIRED)

#compile your rafale project
RAFALE_PROJECT(example)
