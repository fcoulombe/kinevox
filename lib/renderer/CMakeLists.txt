SET(ProjectName "renderer")
MESSAGE("Processing... " ${ProjectName})
ProcessDependencies(${ProjectName})

INCLUDE(glew)
add_definitions(-DENABLE_FIX_PIPELINE=1)
include_directories(${DEP_INC} ${glew_INCLUDE_DIR})
include_directories(".")

file(GLOB ${ProjectName}_src "${ProjectName}/*.h" "*.cpp")

add_library(${ProjectName} ${${ProjectName}_src})