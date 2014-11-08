SET(ProjectName "crashcar")
MESSAGE("Processing... " ${ProjectName})
ProcessDependencies(${ProjectName})
include_directories(${DEP_INC})
include_directories(".")

file(GLOB_RECURSE ${ProjectName}_src "src/*.h" "src/*.cpp")
ProcessData("data")
Executable(${ProjectName})

