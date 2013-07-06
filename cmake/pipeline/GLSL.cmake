SET(OUT_GLSL_DIR ${CMAKE_CURRENT_BINARY_DIR}/data/GLSL/)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/GLSL/)
file(GLOB glslFiles ${dataPath}/*.glsl)
foreach(src ${glslFiles})
	STRING(REPLACE ${dataPath} ${OUT_GLSL_DIR} DstFile ${src})
	add_custom_command(
		OUTPUT ${DstFile}
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${src} ${DstFile}
		DEPENDS ${src} )
	list(APPEND DstFiles ${DstFile})
endforeach()
SET(DATA_DEP ${DATA_DEP} ${DstFiles})
