FIND_PACKAGE(glew)
SET(GLFW_ROOT "${CMAKE_CURRENT_LIST_DIR}")

SET(GLFW_INCLUDE_DIR ${GLFW_ROOT}/include)

FIND_LIBRARY(
  GLFW_LIBRARY 
  NAMES 
    GLFW
  PATHS
    ${GLFW_ROOT}/lib/${KINEVOX_ARCHITECTURE}
  NO_DEFAULT_PATHS )

SET(glfw_INCLUDE_DIR ${GLFW_INCLUDE_DIR} ${glew_INCLUDE_DIR})
list(APPEND glfw_LIBS ${GLFW_LIBRARY} )
list(APPEND glfw_LIBS ${glew_LIBS} )

#SET(glfw_LIBS ${GLFW_LIBRARY} )
