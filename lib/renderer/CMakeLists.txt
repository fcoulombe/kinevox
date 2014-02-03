SET(ProjectName "renderer")
MESSAGE("Processing... " ${ProjectName})
ProcessDependencies(${ProjectName})

include_directories(${DEP_INC})
include_directories(".")

file(GLOB ${ProjectName}_src "${ProjectName}/*.h" "*.cpp")

file(GLOB ${ProjectName}_GL_src "${GFXAPI_NAME}/*.cpp")
file(GLOB ${ProjectName}GFX_INCLUDE "${ProjectName}/${GFXAPI_NAME}/*.h")
SOURCE_GROUP(${GFXAPI_NAME} FILES ${${ProjectName}_GL_src} ${${ProjectName}GFX_INCLUDE} )
file(WRITE "${PROJECT_BINARY_DIR}/rendererconf.h" "#pragma once\n" )
FOREACH(header ${${ProjectName}GFX_INCLUDE})
	get_filename_component(fileName ${header} NAME)
	string(REPLACE ".h" "" fileName ${fileName})
	string(REPLACE ${GFXAPI_NAME} "" fileName ${fileName})
	file(APPEND "${PROJECT_BINARY_DIR}/rendererconf.h" "#define GFXAPI_${fileName}_H \"${ProjectName}/${GFXAPI_NAME}/${GFXAPI_NAME}${fileName}.h\"\n" )
	file(APPEND "${PROJECT_BINARY_DIR}/rendererconf.h" "#define I${fileName} ${GFXAPI_NAME}${fileName}\n" )
	
	#message("GFX: " ${fileName})
ENDFOREACH()
add_library(${ProjectName} ${${ProjectName}_src} ${${ProjectName}_GL_src} ${${ProjectName}GFX_INCLUDE})
SET_TARGET_PROPERTIES( ${ProjectName} PROPERTIES FOLDER Lib)