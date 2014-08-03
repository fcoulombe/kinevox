SET(DepName "renderer")
ProcessDependencies(${DepName})

get_filename_component(RENDERER_ROOT ${CMAKE_CURRENT_LIST_DIR}/../ REALPATH)
SET(RENDERER_INCLUDE_DIR ${RENDERER_ROOT} )


SET(renderer_INCLUDE_DIR ${RENDERER_INCLUDE_DIR} ${DEP_INC} ${glew_INCLUDE_DIR})
list(APPEND renderer_LIBS renderer)
list(APPEND renderer_LIBS ${DEP_LIBS} )
list(APPEND renderer_LIBS ${glew_LIBS})


#SET(renderer_LIBS renderer ${DEP_LIBS} ${glew_LIBS})
