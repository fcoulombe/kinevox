SET(OUT_CONF_DIR ${CMAKE_CURRENT_BINARY_DIR}/data/Conf/)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/Conf/)
file(GLOB confFiles ${dataPath}/*.txt)
foreach(src ${confFiles})
	STRING(REPLACE ${dataPath} ${OUT_CONF_DIR} DstFile ${src})
	add_custom_command(
		OUTPUT ${DstFile}
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${src} ${DstFile}
		DEPENDS ${src} )
	list(APPEND DstFiles ${DstFile})
endforeach()
SET(DATA_DEP ${DATA_DEP} ${DstFiles})
