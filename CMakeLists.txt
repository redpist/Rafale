#set gcc version first
SET(CMAKE_CXX_COMPILER g++)

## Compiler version check.
## Calling directly g++-x.y is a bad solution since some distros lack symlinks with version number.
execute_process(COMMAND         g++ -dumpversion
                COMMAND         tr -d .
                OUTPUT_VARIABLE GCC_VERSION
                OUTPUT_STRIP_TRAILING_WHITESPACE)

IF(${GCC_VERSION} LESS "460")
        MESSAGE(SEND_ERROR "gcc version must be >= 4.6")
ENDIF()

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
