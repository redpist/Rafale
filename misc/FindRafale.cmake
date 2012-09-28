##################
## Copyright (c) 2011, Jeremy Lecerf
## All rights reserved.
##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions are met:
##     * Redistributions of source code must retain the above copyright
##       notice, this list of conditions and the following disclaimer.
##     * Redistributions in binary form must reproduce the above copyright
##       notice, this list of conditions and the following disclaimer in the
##       documentation and/or other materials provided with the distribution.
##     * Neither the name of R0x nor the
##       names of its contributors may be used to endorse or promote products
##       derived from this software without specific prior written permission.
##
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
## AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
## IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
## ARE DISCLAIMED. IN NO EVENT SHALL JEREMY LECERF BE LIABLE FOR ANY DIRECT,
## INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
## (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
## LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
## ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
## (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
##################

FUNCTION(RAFALE_PROJECT project_name)
  EXECUTE_PROCESS(COMMAND         ${CMAKE_CXX_COMPILER} -dumpversion
                COMMAND         tr -d .
                OUTPUT_VARIABLE GCC_VERSION
                OUTPUT_STRIP_TRAILING_WHITESPACE)

  IF(${GCC_VERSION} LESS "47")
        MESSAGE(SEND_ERROR "gcc version must be >= 4.7")
  ENDIF()


  PROJECT(${project_name})

  SET(RAFALE_BUILD_TYPE $ENV{RAFALE_BUILD_TYPE})
  SET(RAFALE_BUILD_ENV $ENV{RAFALE_BUILD_ENV})

  IF (RAFALE_BUILD_TYPE STREQUAL "RELEASE")
     SET(RAFALE_SERVICE_NAME ${PROJECT_NAME})
  ELSE()
     SET(RAFALE_SERVICE_NAME "${PROJECT_NAME}-beta")
  ENDIF()

  SET(RAFALE_INTERNAL_DIR ${PROJECT_SOURCE_DIR}/.rafale)
  SET(RAFALE_BUILD_DIR ${RAFALE_INTERNAL_DIR}/build)
  SET(RAFALE_BUILD_DIR_VIEWS ${RAFALE_BUILD_DIR}/views)
  SET(RAFALE_SOURCE_DIR ${PROJECT_SOURCE_DIR})
  SET(RAFALE_SOURCE_DIR_VIEWS ${RAFALE_SOURCE_DIR}/views)
  SET(RAFALE_SOURCE_DIR_CONTROLLERS ${RAFALE_SOURCE_DIR}/controllers)
  SET(RAFALE_SOURCE_DIR_MODELS ${RAFALE_SOURCE_DIR}/models)
  SET(RAFALE_VIEW_COMPILER rafale-view)

  FILE(GLOB_RECURSE RAFALE_SOURCE_FILES_MODELS ${RAFALE_SOURCE_DIR_MODELS}/*.cc ${RAFALE_SOURCE_DIR_MODELS}/*.hh)


  FILE(GLOB_RECURSE RAFALE_SOURCE_FILES_CONTROLLERS ${RAFALE_SOURCE_DIR_CONTROLLERS}/*.hh)

  FILE(MAKE_DIRECTORY ${RAFALE_INTERNAL_DIR})
  FILE(MAKE_DIRECTORY ${RAFALE_BUILD_DIR})
  FILE(MAKE_DIRECTORY ${RAFALE_BUILD_DIR_VIEWS})

  STRING(LENGTH ${RAFALE_SOURCE_DIR_CONTROLLERS}/ RAFALE_SOURCE_DIR_CONTROLLERS_LENGTH)

  FOREACH(RAFALE_SOURCE_FILES_CONTROLLER ${RAFALE_SOURCE_FILES_CONTROLLERS})
    GET_FILENAME_COMPONENT(RAFALE_SOURCE_FILES_ABSOLUTE_CONTROLLER_PATH ${RAFALE_SOURCE_FILES_CONTROLLER} ABSOLUTE)
    GET_FILENAME_COMPONENT(RAFALE_SOURCE_FILES_CONTROLLER_NAME_WE ${RAFALE_SOURCE_FILES_CONTROLLER} NAME_WE)

    STRING(LENGTH ${RAFALE_SOURCE_FILES_ABSOLUTE_CONTROLLER_PATH} RAFALE_SOURCE_FILES_ABSOLUTE_CONTROLLER_PATH_LENGTH)
    MATH(EXPR RAFALE_SOURCE_FILES_CONTROLLER_LENGTH "${RAFALE_SOURCE_FILES_ABSOLUTE_CONTROLLER_PATH_LENGTH} - ${RAFALE_SOURCE_DIR_CONTROLLERS_LENGTH}")

    STRING(SUBSTRING ${RAFALE_SOURCE_FILES_ABSOLUTE_CONTROLLER_PATH} ${RAFALE_SOURCE_DIR_CONTROLLERS_LENGTH} ${RAFALE_SOURCE_FILES_CONTROLLER_LENGTH} RAFALE_SOURCE_FILES_RELATIVE_CONTROLLER_PATH)

    STRING(LENGTH ${RAFALE_SOURCE_FILES_RELATIVE_CONTROLLER_PATH} RAFALE_SOURCE_FILES_RELATIVE_CONTROLLER_PATH_LENGTH)

    GET_FILENAME_COMPONENT(RAFALE_SOURCE_FILES_CONTROLLER_NAME_EXT ${RAFALE_SOURCE_FILES_RELATIVE_CONTROLLER_PATH} EXT)

    STRING(LENGTH ${RAFALE_SOURCE_FILES_CONTROLLER_NAME_EXT} RAFALE_SOURCE_FILES_CONTROLLER_NAME_EXT_LENGTH)

    MATH(EXPR RAFALE_SOURCE_FILES_RELATIVE_CONTROLLER_PATH_WE_LENGTH "${RAFALE_SOURCE_FILES_RELATIVE_CONTROLLER_PATH_LENGTH} - ${RAFALE_SOURCE_FILES_CONTROLLER_NAME_EXT_LENGTH}")

    STRING(SUBSTRING ${RAFALE_SOURCE_FILES_RELATIVE_CONTROLLER_PATH} 0 ${RAFALE_SOURCE_FILES_RELATIVE_CONTROLLER_PATH_WE_LENGTH} RAFALE_SOURCE_FILES_RELATIVE_CONTROLLER_PATH_WE)

    GET_FILENAME_COMPONENT(RAFALE_SOURCE_FILES_RELATIVE_CONTROLLER_SUBDIR ${RAFALE_SOURCE_FILES_RELATIVE_CONTROLLER_PATH} PATH)


    STRING(LENGTH "${RAFALE_SOURCE_FILES_RELATIVE_CONTROLLER_SUBDIR}" RAFALE_SOURCE_FILES_RELATIVE_CONTROLLER_SUBDIR_LENGTH)

    IF (${RAFALE_SOURCE_FILES_RELATIVE_CONTROLLER_SUBDIR_LENGTH})
      FILE(MAKE_DIRECTORY ${RAFALE_BUILD_DIR_VIEWS}/${RAFALE_SOURCE_FILES_RELATIVE_CONTROLLER_SUBDIR})
    ENDIF (${RAFALE_SOURCE_FILES_RELATIVE_CONTROLLER_SUBDIR_LENGTH})


    ADD_CUSTOM_COMMAND(
      OUTPUT ${RAFALE_BUILD_DIR_VIEWS}/${RAFALE_SOURCE_FILES_RELATIVE_CONTROLLER_PATH_WE}.inc
      COMMAND ${RAFALE_VIEW_COMPILER}
      ARGS ${RAFALE_SOURCE_DIR_VIEWS}/${RAFALE_SOURCE_FILES_RELATIVE_CONTROLLER_PATH_WE}.view -o ${RAFALE_BUILD_DIR_VIEWS}/${RAFALE_SOURCE_FILES_RELATIVE_CONTROLLER_PATH_WE}.inc
      DEPENDS ${RAFALE_SOURCE_DIR_VIEWS}/${RAFALE_SOURCE_FILES_RELATIVE_CONTROLLER_PATH_WE}.view
  )

  LIST(APPEND RAFALE_BUILD_VIEWS ${RAFALE_BUILD_DIR_VIEWS}/${RAFALE_SOURCE_FILES_RELATIVE_CONTROLLER_PATH_WE}.inc)


  # ADD_DEPENDENCIES(${RAFALE_SOURCE_FILES_CONTROLLER}
  #                 ${RAFALE_BUILD_DIR_VIEWS}/${RAFALE_SOURCE_FILES_RELATIVE_CONTROLLER_PATH_WE}.inc)


  ENDFOREACH(RAFALE_SOURCE_FILES_CONTROLLER)



  INCLUDE_DIRECTORIES(${RAFALE_INC_DIR}
                     ${PROJECT_SOURCE_DIR}
                     ${RAFALE_BUILD_DIR}
                    )
  LINK_DIRECTORIES(${RAFALE_LIB_DIR})


  # add main target, the executable file
  ADD_EXECUTABLE(${RAFALE_SERVICE_NAME}
          main.cc
          ${RAFALE_BUILD_VIEWS}
          ${RAFALE_SOURCE_FILES_MODELS}
          ${RAFALE_SOURCE_FILES_CONTROLLERS}
          )

  # ADD_DEPENDENCIES(${RAFALE_SERVICE_NAME}
  #         ${RAFALE_SOURCE_FILES_MODELS}
  #         )


  TARGET_LINK_LIBRARIES(${RAFALE_SERVICE_NAME}
                        rafale
  		                          mysqlclient
                        mysqlcppconn
                        fcgi
                        curl
                        )
ENDFUNCTION(RAFALE_PROJECT)
