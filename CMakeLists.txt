cmake_minimum_required (VERSION 2.8)
project (kinevox)
SET(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "limited configs"  FORCE)
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    option(ENABLE_VLD "enable visual leak detector if installed" ON)
endif()

option(USE_OPENRL "Enables Open Ray Tracing Library. SDK from caustic must be installed" OFF)

SET(GFXAPI_TO_USE "OpenGL3" CACHE STRING "library to use for graphics")
SET_PROPERTY(CACHE GFXAPI_TO_USE PROPERTY STRINGS OpenGL3 OpenGL OpenGLES2 OpenGLES3 D3D) 

enable_testing()
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")
INCLUDE(kinevoxconf)
INCLUDE(kinevox)

FindCMAKE(${PROJECT_SOURCE_DIR}/cmake)
FindCMAKE(${PROJECT_SOURCE_DIR}/lib)
FindCMAKE(${PROJECT_SOURCE_DIR}/program)
FindCMAKE(${PROJECT_SOURCE_DIR}/3rdParty)
foreach(p in ${pathList})
	SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${p})
endforeach()

FindCMakeLists(${PROJECT_SOURCE_DIR}/lib)
FindCMakeLists(${PROJECT_SOURCE_DIR}/program)
add_custom_target(build_and_test ${CMAKE_CTEST_COMMAND} -C $(ConfigurationName) --no-label-summary --output-on-failure)
foreach(makefile ${cmakelist})
	add_subdirectory(${makefile}) 
endforeach()
#add_subdirectory(lib/applayer/unittest) 
#add_subdirectory(program/tools/common/unittest) 


# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_target(doc
	${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMENT "Generating API documentation with Doxygen" VERBATIM
	)
endif(DOXYGEN_FOUND)

include(ExternalProject)
ExternalProject_Add(
   tools
   PREFIX "${CMAKE_CURRENT_BINARY_DIR}/tools"
   DOWNLOAD_COMMAND ""
   SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tools 
   #INSTALL_DIR ${${depname}_INSTALL_DIR}
   #CMAKE_ARGS ${${depname}_CMAKE_ARGS}
)
