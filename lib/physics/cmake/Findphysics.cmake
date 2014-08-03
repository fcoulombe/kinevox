SET(DepName "physics")
ProcessDependencies(${DepName})

get_filename_component(PHYSICS_ROOT ${CMAKE_CURRENT_LIST_DIR}/../ REALPATH)
SET(PHYSICS_INCLUDE_DIR ${PHYSICS_ROOT} )


SET(physics_INCLUDE_DIR ${PHYSICS_INCLUDE_DIR} ${DEP_INC} )
list(APPEND physics_LIBS physics)
list(APPEND physics_LIBS ${DEP_LIBS} )

