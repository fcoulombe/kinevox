SET(DepName "applayer")
ProcessDependencies(${DepName})

SET(APPLAYER_ROOT ${CMAKE_CURRENT_LIST_DIR}/../)
SET(APPLAYER_INCLUDE_DIR ${APPLAYER_ROOT} )


SET(applayer_INCLUDE_DIR ${APPLAYER_INCLUDE_DIR} ${DEP_INC} )
list(APPEND applayer_LIBS applayer)
list(APPEND applayer_LIBS ${DEP_LIBS} )
#SET(applayer_LIBS applayer ${DEP_LIBS} )
