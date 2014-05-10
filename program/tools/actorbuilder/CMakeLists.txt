SET(ProjectName "actorbuilder")
MESSAGE("Processing... " ${ProjectName})
ProcessDependencies(${ProjectName})

set (CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.0.2\\5.0.2\\msvc2012_64\\")
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Widgets REQUIRED)


include_directories(${DEP_INC})
file(GLOB ${ProjectName}_src "src/*.cpp" "src/*.h")
ProcessData("data")

Executable(${ProjectName})
SET_TARGET_PROPERTIES( ${ProjectName} PROPERTIES FOLDER Tool)
qt5_use_modules(${ProjectName} Widgets)

