cmake_minimum_required (VERSION 2.8)
project (kinevox_tools)
SET(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "limited configs"  FORCE)
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    option(ENABLE_VLD "enable visual leak detector if installed" ON)
endif()
SET(GFXAPI_TO_USE "OpenGL3" CACHE STRING "library to use for graphics")
SET_PROPERTY(CACHE GFXAPI_TO_USE PROPERTY STRINGS OpenGL3 OpenGL OpenGLES2 OpenGLES3 D3D) 

enable_testing()
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/../cmake/")
INCLUDE(kinevoxconf)
INCLUDE(kinevox)


FindCMAKE(${PROJECT_SOURCE_DIR}/../cmake)
FindCMAKE(${PROJECT_SOURCE_DIR}/../3rdParty)
FindCMAKE(${PROJECT_SOURCE_DIR}/../lib/gcl)
FindCMAKE(${PROJECT_SOURCE_DIR}/)
foreach(p in ${pathList})
	SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${p})
endforeach()

add_subdirectory(${PROJECT_SOURCE_DIR}/../lib/gcl/gcl "${CMAKE_CURRENT_BINARY_DIR}/gcl")
FindCMakeLists(${PROJECT_SOURCE_DIR}/common)
FindCMakeLists(${PROJECT_SOURCE_DIR}/fontconverter)
FindCMakeLists(${PROJECT_SOURCE_DIR}/meshconverter)
FindCMakeLists(${PROJECT_SOURCE_DIR}/worldconverter)
add_custom_target(build_and_test ${CMAKE_CTEST_COMMAND} -C $(ConfigurationName) --no-label-summary --output-on-failure)
foreach(makefile ${cmakelist})
	add_subdirectory(${makefile}) 
endforeach()


