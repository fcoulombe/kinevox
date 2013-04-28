SET(ProjectName "renderer")
MESSAGE("Processing... " ${ProjectName})
ProcessDependencies(${ProjectName})

include_directories(${DEP_INC})
include_directories(".")

file(GLOB ${ProjectName}_src "${ProjectName}/*.h" "*.cpp")

file(GLOB ${ProjectName}_GL_src "${ProjectName}/GL/*.h" "GL/*.cpp")

add_library(${ProjectName} ${${ProjectName}_src} ${${ProjectName}_GL_src})