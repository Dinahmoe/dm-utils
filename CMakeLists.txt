CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

SET(DM_UTILS_VERSION_MAJOR 1)
SET(DM_UTILS_VERSION_MINOR 0)
SET(DM_UTILS_VERSION_BUILD 0)

OPTION(BUILD_DM_UTILS_TEST OFF)

MESSAGE(STATUS "Configuring dm-utils library")

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../dm-cmake)
INCLUDE(SetTargetOptions)
INCLUDE(SetPlatformDefines)
SET_PLATFORM_DEFINES()


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../dm-log/include)
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/include/")

FILE(GLOB dm-utils_include_files "${CMAKE_CURRENT_SOURCE_DIR}/include/*.*")
SET(dm-utils_src "")

IF (APPLE)
  IF("${BUILD_FOR}" STREQUAL "ios" )
    SET(dm-utils_platform
      ${CMAKE_CURRENT_SOURCE_DIR}/src/apple/StringUtilitiesMac.mm)
  ELSE("${BUILD_FOR}" STREQUAL "ios" )
    SET(dm-utils_platform
      ${CMAKE_CURRENT_SOURCE_DIR}/src/apple/StringUtilitiesMac.mm)
  ENDIF("${BUILD_FOR}" STREQUAL "ios" )
  SET(DM_UTILS_LINK_FLAGS "-framework Foundation" CACHE INTERNAL "dm-utils linker flags")
ELSE(APPLE)
  SET(dm-utils_platform
      ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/StringUtilitiesLinux.cpp)
ENDIF(APPLE)

ADD_LIBRARY(dm-utils
    ${dm-utils_include_files}
    ${dm-utils_src}
    ${dm-utils_platform})
FSET_TARGET_OPTIONS(dm-utils "")

# ADD_SUBDIRECTORY(test)
