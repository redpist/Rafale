#set gcc version first
SET(CMAKE_CXX_COMPILER g++-4.7)

PROJECT(rafale)

add_subdirectory(rafale-view)

cmake_minimum_required(VERSION 2.8)

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
    ${${PROJECT_NAME}_SRC_DIRECTORY}/client.cc
    ${${PROJECT_NAME}_SRC_DIRECTORY}/sessions.cc
    ${${PROJECT_NAME}_SRC_DIRECTORY}/environment.cc
    ${${PROJECT_NAME}_SRC_DIRECTORY}/server.cc
    ${${PROJECT_NAME}_SRC_DIRECTORY}/controller.cc
    ${${PROJECT_NAME}_SRC_DIRECTORY}/curl.cc
    ${${PROJECT_NAME}_SRC_DIRECTORY}/tools.cc
    ${${PROJECT_NAME}_SRC_DIRECTORY}/plugin/paypal.cc
		)

SET(CMAKE_CXX_FLAGS "-Wextra -Wall -std=c++11 -pthread")

#Compilation Section
add_library (${PROJECT_NAME}
							${${PROJECT_NAME}_SRCS})

#link section
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
	                    mysqlclient
                      mysqlcppconn
                      fcgi
                      fcgi++
                      curl
                      )

INSTALL(
        TARGETS ${PROJECT_NAME} DESTINATION lib
        )

INSTALL(
        FILES "misc/FindRafale.cmake" DESTINATION /usr/share/cmake-2.8/Modules/
        )

INSTALL(
        FILES "bin/rafale-mail" DESTINATION bin PERMISSIONS  OWNER_WRITE OWNER_READ OWNER_EXECUTE
     GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
        )


INSTALL(
        FILES "${${PROJECT_NAME}_HEADER_DIRECTORY}/rafale.h" DESTINATION include
        )

INSTALL(
        DIRECTORY "${${PROJECT_NAME}_HEADER_DIRECTORY}/rafale" DESTINATION include
        )
