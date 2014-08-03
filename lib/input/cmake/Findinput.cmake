SET(DepName "input")
ProcessDependencies(${DepName})

get_filename_component(INPUT_ROOT ${CMAKE_CURRENT_LIST_DIR}/../ REALPATH)
SET(INPUT_INCLUDE_DIR ${INPUT_ROOT} )


SET(input_INCLUDE_DIR ${INPUT_INCLUDE_DIR} ${DEP_INC} )
list(APPEND input_LIBS input)
list(APPEND input_LIBS ${DEP_LIBS} )

#SET(input_LIBS input ${DEP_LIBS} )
