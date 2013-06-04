IF(${WIN32})
	FIND_PACKAGE(zlib)
	IF(${IS_MSVC})
		SET(PNG_ROOT "${PROJECT_SOURCE_DIR}/3rdParty/libpng/")
		SET(LPATH ${PNG_ROOT}/lib/${KINEVOX_ARCHITECTURE})
	ELSEIF(${IS_GNU})
		SET(PNG_ROOT "${MSYS_PATH}/local")		
		SET(LPATH ${PNG_ROOT}/lib/)
	ENDIF()

	SET(PNG_ROOT "${PROJECT_SOURCE_DIR}/3rdParty/libpng/")
	FIND_PATH(
		  PNG_INCLUDE_DIR png.h
		  PATHS ${PNG_ROOT}/include)
		
	FindLibrary(PNG_LIBRARY libpng15 ${LPATH})

		
	SET(libpng_INCLUDE_DIR ${PNG_INCLUDE_DIR} ${zlib_INCLUDE_DIR})
	list(APPEND libpng_LIBS ${zlib_LIBS} )
	list(APPEND libpng_LIBS ${PNG_LIBRARY})
	#message("png!!!" ${PNG_LIBRARY})
ELSE()
	FIND_PACKAGE(PNG)
	SET(libpng_INCLUDE_DIR ${PNG_INCLUDE_DIR})
	list(APPEND libpng_LIBS ${PNG_LIBRARY})
ENDIF()

