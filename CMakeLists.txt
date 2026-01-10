cmake_minimum_required(VERSION 3.28)
message("Using toolchain file ${CMAKE_TOOLCHAIN_FILE}.")

project(
  SurvivalModeImproved
  VERSION 1.6.0
  LANGUAGES CXX
)

#Enable for 1130 changes
set(SKYRIM_SUPPORT_AE ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)
set(CMAKE_OPTIMIZE_DEPENDENCIES ON)
set(SKSE_SUPPORT_XBYAK ON)
set(ENABLE_SKYRIM_VR OFF)

include(GNUInstallDirs)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/Version.h.in
	${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
	@ONLY
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
  ${CMAKE_CURRENT_BINARY_DIR}/version.rc
  @ONLY
)

set(PUBLIC_HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME})

# Add headers
file(
  GLOB_RECURSE
  headers
  ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
  ${CMAKE_CURRENT_BINARY_DIR}/include/Version.h

)
list(REMOVE_ITEM headers ${CMAKE_CURRENT_SOURCE_DIR}/include/PCH.h)

# Add sources
file(
  GLOB_RECURSE
  sources
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
  ${CMAKE_CURRENT_BINARY_DIR}/version.rc
  ${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
)

source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR}
  FILES ${headers} ${sources}
)

set(BUILD_TESTS OFF)
add_subdirectory(extern/CommonLibSSE-NG)
include(extern/CommonLibSSE-NG/cmake/CommonLibSSE.cmake)

add_commonlibsse_plugin(
  ${PROJECT_NAME}
  AUTHOR colinswrath
  SOURCES ${headers} ${sources}
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

find_path(SIMPLEINI_INCLUDE_DIRS "SimpleIni.h")

set (SOURCES ${headers} ${sources})

# target_link_libraries(
# ${PROJECT_NAME}
# PRIVATE
# SomeLibrary::SomeLibrary
# )
target_include_directories(
  ${PROJECT_NAME}
  PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  $<INSTALL_INTERFACE:src>
  ${SIMPLEINI_INCLUDE_DIRS}
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_precompile_headers(
  ${PROJECT_NAME}
  PRIVATE
  include/PCH.h
  ${SIMPLEINI_INCLUDE_DIRS}/SimpleIni.h
)

install(
  DIRECTORY ${PUBLIC_HEADER_DIR}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
  TARGETS ${PROJECT_NAME}
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set(BUILD_NAME Debug)
else()
  set(BUILD_NAME Release)
endif()

if(SKYRIM_SUPPORT_AE)
	message("\tBuilding 1130")
  set(BUILD_NAME ${BUILD_NAME}1130)
endif()

add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND
  ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/contrib/Plugin${BUILD_NAME}/skse/plugins
  COMMAND
  ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${CMAKE_CURRENT_SOURCE_DIR}/contrib/Plugin${BUILD_NAME}/skse/plugins
  COMMAND
  ${CMAKE_COMMAND} -E copy $<TARGET_PDB_FILE:${PROJECT_NAME}> ${CMAKE_CURRENT_SOURCE_DIR}/contrib/Plugin${BUILD_NAME}/skse/plugins
  COMMAND
  ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/contrib/Config/${PROJECT_NAME}.ini ${CMAKE_CURRENT_SOURCE_DIR}/contrib/Plugin${BUILD_NAME}/skse/plugins
)

file(GLOB_RECURSE OUTPUT_DLLS ${CMAKE_CURRENT_SOURCE_DIR}/contrib/**/*.dll)
file(GLOB_RECURSE OUTPUT_PDBS ${CMAKE_CURRENT_SOURCE_DIR}/contrib/**/*.pdb)

set_property(
  TARGET ${PROJECT_NAME}
  APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${OUTPUT_DLLS} ${OUTPUT_PDBS}
)
