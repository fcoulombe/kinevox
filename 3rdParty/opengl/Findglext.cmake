FIND_PACKAGE(opengl)

SET(glext_ROOT "${PROJECT_SOURCE_DIR}/3rdParty/opengl/")
SET(glext_INCLUDE_DIR ${glext_ROOT}/include)


SET(glext_INCLUDE_DIR ${glext_INCLUDE_DIR} )
list(APPEND glext_LIBS ${OPENGL_gl_LIBRARY})
list(APPEND glext_LIBS ${OPENGL_glu_LIBRARY})
#SET(glext_LIBS ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
