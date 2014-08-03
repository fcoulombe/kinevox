SET(ProjectName "test_common")
ProcessDependencies(${ProjectName})
include_directories(${DEP_INC})
include_directories(".")

file(GLOB ${ProjectName}_src "*.h" "*.cpp")
SET(OUT_DATA_MISC ${CMAKE_CURRENT_BINARY_DIR}/datamisc)
add_custom_command(
		OUTPUT ${OUT_DATA_MISC}
		COMMAND ${CMAKE_COMMAND} -E copy_directory  "${CMAKE_CURRENT_LIST_DIR}/datamisc" ${OUT_DATA_MISC} )
SET(DATA_DEP ${DATA_DEP} ${OUT_DATA_MISC})
message("common!!! "  ${DATA_DEP} )
Executable(${ProjectName})

Test(${ProjectName})