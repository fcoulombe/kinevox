SET(ProjectName "physics")
MESSAGE("Processing... " ${ProjectName})
ProcessDependencies(${ProjectName})

include_directories(${DEP_INC} )
include_directories(".")

file(GLOB ${ProjectName}_src "${ProjectName}/*.h" "*.cpp")

add_library(${ProjectName} ${${ProjectName}_src})
SET_TARGET_PROPERTIES( ${ProjectName} PROPERTIES FOLDER Lib)