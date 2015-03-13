SET(DepName "script")
ProcessDependencies(${DepName})

get_filename_component(SCRIPT_ROOT ${CMAKE_CURRENT_LIST_DIR}/../ REALPATH)
SET(SCRIPT_INCLUDE_DIR ${SCRIPT_ROOT} )
SET(SCRIPT_INCLUDE_DIR ${SCRIPT_INCLUDE_DIR} ${SCRIPT_ROOT}/lua-5.3.0/src/ )


add_definitions(-DLUA_COMPAT_ALL)

SET(script_INCLUDE_DIR ${SCRIPT_INCLUDE_DIR} ${DEP_INC} )
list(APPEND script_LIBS script)
list(APPEND script_LIBS ${DEP_LIBS} )

#SET(script_LIBS script ${DEP_LIBS} )
