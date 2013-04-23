INCLUDE(CheckTypeSize)
CHECK_TYPE_SIZE (size_t SIZE_T_SIZE)

IF(${SIZE_T_SIZE} EQUAL 4)
SET(KINEVOX_ARCHITECTURE "32bit")
else()
SET(KINEVOX_ARCHITECTURE "64bit")
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
   SET(OS "MACOSX")
   
   SET(IS_OPENRL_ENABLED "0")
ENDIF()
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
   SET(OS "LINUX")
   SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-include kinevoxconf.h")
   
   SET(IS_OPENRL_ENABLED "0")
ENDIF()
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
   SET(OS "WIN32")
   SET(IS_OPENRL_ENABLED "1")
ENDIF()

IF(${MSVC})
   SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "/FIkinevoxconf.h")
   SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "/EHsc")
ENDIF(${MSVC})


configure_file (
  "${PROJECT_SOURCE_DIR}/cmake/kinevoxconf.h.in"
  "${PROJECT_BINARY_DIR}/kinevoxconf.h"
  )
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/3rdParty/include")
