SET(ProjectName "applayer")
MESSAGE("Processing... " ${ProjectName})
ProcessDependencies(${ProjectName})

include_directories(${DEP_INC} )
include_directories(".")

file(GLOB ${ProjectName}_src "${ProjectName}/*.h" "*.cpp")
FOREACH(f ${${ProjectName}_src})
    string(FIND ${f} "Component" pos)
    if (NOT ${pos} EQUAL -1)
        #message("pos: " ${f})
        SOURCE_GROUP("Component" FILES ${f} )
    endif()
    
endforeach()
add_library(${ProjectName} ${${ProjectName}_src})
SET_TARGET_PROPERTIES( ${ProjectName} PROPERTIES FOLDER Lib)