SET(ProjectName "test_kinetestlib")
ProcessDependencies(${ProjectName})
include_directories(${DEP_INC})
include_directories(".")

file(GLOB ${ProjectName}_src "*.h" "*.cpp")

ProcessData("data")

Executable(${ProjectName})
Test(${ProjectName})
