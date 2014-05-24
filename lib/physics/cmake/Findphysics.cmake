SET(DepName "physics")
ProcessDependencies(${DepName})

SET(PHYSICS_ROOT ${CMAKE_CURRENT_LIST_DIR}/../)
SET(PHYSICS_INCLUDE_DIR ${PHYSICS_ROOT} )


SET(physics_INCLUDE_DIR ${PHYSICS_INCLUDE_DIR} ${DEP_INC} )
list(APPEND physics_LIBS physics)
list(APPEND physics_LIBS ${DEP_LIBS} )

