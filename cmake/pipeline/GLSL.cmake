SET(OUT_GLSL_DIR ${CMAKE_CURRENT_BINARY_DIR}/data/GLSL/)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/GLSL/)
IF(${GFXAPI_TO_USE} STREQUAL "OpenGL3")
    file(GLOB glslFiles ${dataPath}/*.glsl3)
ELSEIF(${GFXAPI_TO_USE} STREQUAL "OpenGL")
    file(GLOB glslFiles ${dataPath}/*.glsl)
ELSE()
    message( FATAL_ERROR "you can't use shaders with this gfx api" )
ENDIF()
foreach(src ${glslFiles})
	STRING(REPLACE ${dataPath} ${OUT_GLSL_DIR} DstFile ${src})
	add_custom_command(
		OUTPUT ${DstFile}
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${src} ${DstFile}
		DEPENDS ${src} )
	list(APPEND DstFiles ${DstFile})
endforeach()
SET(DATA_DEP ${DATA_DEP} ${DstFiles})
