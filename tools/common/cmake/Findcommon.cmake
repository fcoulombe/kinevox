SET(DepName "common")
ProcessDependencies(${DepName})

get_filename_component(common_ROOT ${CMAKE_CURRENT_LIST_DIR}/../ REALPATH)
SET(common_INCLUDE_DIR ${common_ROOT} )


SET(common_INCLUDE_DIR ${common_INCLUDE_DIR} ${DEP_INC})
list(APPEND common_LIBS common)
list(APPEND common_LIBS ${DEP_LIBS} )

#SET(common_LIBS common ${DEP_LIBS} )
