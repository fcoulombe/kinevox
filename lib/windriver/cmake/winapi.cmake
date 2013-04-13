Set(WINAPI "glfw")

IF(${WINAPI} MATCHES "glfw")
   add_definitions(-DUSE_GLFW)
ENDIF()

IF(${WINAPI} MATCHES "winapi")
   add_definitions(-DUSE_WINAPI)
ENDIF()
