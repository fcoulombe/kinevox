string(COMPARE EQUAL "${CMAKE_BUILD_TYPE}" "" result)
IF(result)
    SET(CMAKE_BUILD_TYPE Debug)
ENDIF()
IF(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    SET(KINEVOX_ARCHITECTURE "32bit")
    SET(SIZE_T_SIZE 4)
ELSE()
    INCLUDE(CheckTypeSize)
    CHECK_TYPE_SIZE (size_t SIZE_T_SIZE)
    
    IF(${SIZE_T_SIZE} EQUAL 4)
        SET(KINEVOX_ARCHITECTURE "32bit")
    else()
        SET(KINEVOX_ARCHITECTURE "64bit")
    ENDIF()
ENDIF()

message("OS: " ${CMAKE_SYSTEM_NAME} ${CMAKE_CXX_COMPILER_ID} ${KINEVOX_ARCHITECTURE})

SET(MSYS_PATH "C:/MinGW/msys/1.0")
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    SET(IS_MSVC 1)
    SET(IS_GNU 0)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    SET(IS_MSVC 0)
    SET(IS_GNU 1)
else()
    message("compiler undef!" ${CMAKE_CXX_COMPILER_ID})
endif()

IF(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
   SET(OS "MACOSX")
ELSEIF(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
   SET(OS "LINUX")
ELSEIF(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
   SET(OS "WIN32")
ELSEIF(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
   SET(OS "ANDROID")
   SET(ANDROID true)
ENDIF()
IF(${GFXAPI_TO_USE} STREQUAL "OpenGL3")
	SET(USE_OPENGL3 "1")
	SET(IS_GLEW_ENABLED 1)
	SET(IS_EGLEW_ENABLED 0)
	SET(GFXAPI_NAME "GL")
ELSEIF(${GFXAPI_TO_USE} STREQUAL "OpenGLES2")
    SET(USE_OPENGL3 "0")
	SET(ES2 ON)
	SET(IS_GLEW_ENABLED 0)
	SET(IS_EGLEW_ENABLED 1)
	SET(GFXAPI_NAME "GLES")
ELSEIF(${GFXAPI_TO_USE} STREQUAL "OpenGLES3")
    SET(USE_OPENGL3 "0")
	SET(ES3 ON)
	SET(IS_GLEW_ENABLED 0)
	SET(IS_EGLEW_ENABLED 1)
	SET(GFXAPI_NAME "GLES")
ELSEIF(${GFXAPI_TO_USE} STREQUAL "OpenGL")
	SET(USE_OPENGL3 "0")
	SET(IS_GLEW_ENABLED 1)
	SET(IS_EGLEW_ENABLED 0)
	SET(GFXAPI_NAME "GL")
ENDIF()
IF(USE_OPENRL)
    SET(IS_OPENRL_ENABLED "1")
ELSE()
    SET(IS_OPENRL_ENABLED "0")
ENDIF()
IF(${MSVC})
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /FIkinevoxconf.h")
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
   SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /RTC1")
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MDd")
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GF")
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GS")
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")

   IF(${KINEVOX_ARCHITECTURE} STREQUAL "32bit")
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4305")  #conversion double to float
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244")  #conversion double to float
   ENDIF()
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4127")  #disable warning about constant condition for asserts
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4702") #unreachable code for template in vertex buffer
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_CRT_SECURE_NO_WARNINGS /DNOMINMAX")
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
   SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEBUG")
ELSEIF(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include kinevoxconf.h")
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wall")
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -g")
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fexceptions")
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -ftrapv")
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fvisibility=hidden")
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Werror")
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wextra")
   IF (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wthread-safety")
       #http://clang.llvm.org/docs/AddressSanitizer.html
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=address")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize-blacklist=${PROJECT_SOURCE_DIR}/blacklist.txt")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fno-optimize-sibling-calls")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fno-omit-frame-pointer")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=integer")
       #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=thread")
       #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=memory")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=undefined")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=undefined-trap")
       #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=dataflow")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=alignment")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=bool")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=bounds")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=enum")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=float-cast-overflow")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=float-divide-by-zero")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=function")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=integer-divide-by-zero")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=nonnull-attribute")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=null")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=object-size")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=return")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=returns-nonnull-attribute")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=shift")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=signed-integer-overflow")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=unreachable")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=unsigned-integer-overflow")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=vla-bound")
       SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=vptr")
       
   ENDIF()
IF(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")   
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=gnu++11")
ELSE()
   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++11")
ENDIF()

   SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -pedantic")
ENDIF()

configure_file (
  "${CMAKE_CURRENT_LIST_DIR}/kinevoxconf.h.in"
  "${PROJECT_BINARY_DIR}/kinevoxconf.h"
  )
include_directories("${PROJECT_BINARY_DIR}")
get_filename_component(THIRD_PARTY_INCLUDE
                       "${CMAKE_CURRENT_LIST_DIR}/../3rdParty/include"
                       REALPATH)

include_directories("${THIRD_PARTY_INCLUDE}")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
