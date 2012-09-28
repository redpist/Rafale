#set gcc version first
SET(CMAKE_CXX_COMPILER g++-4.7)

PROJECT(rafale-view)

cmake_minimum_required(VERSION 2.8)

#Libraries Section
SET(${PROJECT_NAME}_LIB_DIRECTORY lib)
INCLUDE_DIRECTORIES(
                     include/
										)

#Headers
SET(${PROJECT_NAME}_HEADER_DIRECTORY include/)
INCLUDE_DIRECTORIES(
									 ${${PROJECT_NAME}_HEADER_DIRECTORY}
									 )

#Sources
SET(${PROJECT_NAME}_SRC_DIRECTORY src/)
SET(${PROJECT_NAME}_SRCS
    ${${PROJECT_NAME}_SRC_DIRECTORY}/main.cc
    ${${PROJECT_NAME}_SRC_DIRECTORY}/options.cc
    ${${PROJECT_NAME}_SRC_DIRECTORY}/data.cc
		)

SET(CMAKE_CXX_FLAGS "-Wextra -Wall -std=c++11")

#Compilation Section
add_executable(${PROJECT_NAME}
							${${PROJECT_NAME}_SRCS}
							)

INSTALL(
        TARGETS ${PROJECT_NAME} DESTINATION bin
        )

