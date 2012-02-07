#set gcc version first
SET(CMAKE_CXX_COMPILER g++-4.6)

PROJECT(rafale)

cmake_minimum_required(VERSION 2.6)


#Libraries Section
SET(${PROJECT_NAME}_LIB_DIRECTORY lib)
INCLUDE_DIRECTORIES(
										)

LINK_DIRECTORIES(/usr/lib/mysql)


#Headers
SET(${PROJECT_NAME}_HEADER_DIRECTORY include/)
INCLUDE_DIRECTORIES(
									 ${${PROJECT_NAME}_HEADER_DIRECTORY}
									 # ${CMAKE_CURRENT_BINARY_DIR}
									 )

#Sources
SET(${PROJECT_NAME}_SRC_DIRECTORY src/)
SET(${PROJECT_NAME}_SRCS
    ${${PROJECT_NAME}_SRC_DIRECTORY}/main.cc
		)

SET(CMAKE_CXX_FLAGS "-Wextra -Wall -std=c++0x")


#Compilation Section
add_executable(${PROJECT_NAME}
							${${PROJECT_NAME}_SRCS}
							)

#link section
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
	                    mysqlclient_r
                      mysqlcppconn
                      )
