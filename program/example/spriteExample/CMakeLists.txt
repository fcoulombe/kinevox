SET(ProjectName "sprite_example")
MESSAGE("Processing... " ${ProjectName})
ProcessDependencies(${ProjectName})
include_directories(${DEP_INC})
include_directories(".")

file(GLOB_RECURSE ${ProjectName}_src "src/*.h" "src/*.cpp")
ProcessData("../example_data/data")
Executable(${ProjectName})
SET_TARGET_PROPERTIES( ${ProjectName} PROPERTIES FOLDER Example)

