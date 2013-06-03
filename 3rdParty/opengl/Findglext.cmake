
IF(${ES1})
	FIND_PACKAGE(eglew)
	SET(POWERVR_SDK_ROOT "C:\\Imagination\\PowerVR\\GraphicsSDK\\SDK_3.1\\Builds\\")
	SET(GL_INCLUDE ${POWERVR_SDK_ROOT}/Include/)
	
	IF(${KINEVOX_ARCHITECTURE} STREQUAL "32bit")
		SET(GL_LIBS_PATH  ${POWERVR_SDK_ROOT}Windows/x86_32/Lib)
	ELSEIF(${KINEVOX_ARCHITECTURE} STREQUAL "64bit")
		SET(GL_LIBS_PATH  ${POWERVR_SDK_ROOT}Windows/x86_64/Lib)
	ENDIF()
	SET(EGL_LIB ${GL_LIBS_PATH}/libEGL.lib)
	
	SET(GLES_LIB ${GL_LIBS_PATH}/libGLES_CM.lib)
	
	SET(glext_INCLUDE_DIR ${GL_INCLUDE} ${eglew_INCLUDE_DIR})

	list(APPEND glext_LIBS ${eglew_LIBS})
	list(APPEND glext_LIBS ${EGL_LIB})
	list(APPEND glext_LIBS ${GLES_LIB})
	
	list(APPEND glext_DLL ${GL_LIBS_PATH}/libEGL.dll)
	list(APPEND glext_DLL ${GL_LIBS_PATH}/libGLES_CM.dll)
	
ELSEIF(${ES2})
	SET(POWERVR_SDK_ROOT "C:\\Imagination\\PowerVR\\GraphicsSDK\\SDK_3.1\\Builds\\")
	SET(GL_INCLUDE ${POWERVR_SDK_ROOT}/Include/)
	
	IF(${KINEVOX_ARCHITECTURE} STREQUAL "32bit")
		SET(GL_LIBS_PATH  ${POWERVR_SDK_ROOT}Windows/x86_32/Lib)
	ELSEIF(${KINEVOX_ARCHITECTURE} STREQUAL "64bit")
		SET(GL_LIBS_PATH  ${POWERVR_SDK_ROOT}Windows/x86_64/Lib)
	ENDIF()
	SET(EGL_LIB ${GL_LIBS_PATH}/libEGL.lib)
	
	SET(GLES_LIB ${GL_LIBS_PATH}/libGLESv2.lib)
	
	SET(glext_INCLUDE_DIR ${GL_INCLUDE} )

	list(APPEND glext_LIBS ${EGL_LIB})
	list(APPEND glext_LIBS ${GLES_LIB})
	
	list(APPEND glext_DLL ${GL_LIBS_PATH}/libEGL.dll)
	list(APPEND glext_DLL ${GL_LIBS_PATH}/libGLESv2.dll)
ELSEIF(${ES3})
	message("ES3")
ELSE()
	FIND_PACKAGE(OpenGL)
	FIND_PACKAGE(glew)
	
	SET(glext_ROOT "${PROJECT_SOURCE_DIR}/3rdParty/opengl/")
	SET(glext_INCLUDE_DIR ${glext_ROOT}/include ${glew_INCLUDE_DIR})
	
	
	SET(glext_INCLUDE_DIR ${glext_INCLUDE_DIR} )
	list(APPEND glext_LIBS ${OPENGL_gl_LIBRARY})
	list(APPEND glext_LIBS ${OPENGL_glu_LIBRARY})
	list(APPEND glext_LIBS ${glew_LIBS} )
	#SET(glext_LIBS ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
ENDIF()
