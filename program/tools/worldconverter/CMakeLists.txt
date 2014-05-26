SET(ProjectName "worldconverter")
MESSAGE("Processing... " ${ProjectName})
ProcessDependencies(${ProjectName})

include_directories(${DEP_INC})
file(GLOB ${ProjectName}_src "src/*.cpp")

Executable(${ProjectName})
SET_TARGET_PROPERTIES( ${ProjectName} PROPERTIES FOLDER Tool)
