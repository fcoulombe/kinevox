SET(OUT_TEXTURE_DIR ${CMAKE_CURRENT_BINARY_DIR}/data/Texture/)
file(MAKE_DIRECTORY ${OUT_TEXTURE_DIR})
file(GLOB tgaFiles ${dataPath}*.tga)
file(GLOB pngFiles ${dataPath}*.png)
file(GLOB rawFiles ${dataPath}*.raw)
SET(Textures ${pngFiles} ${tgaFiles} ${rawFiles})
foreach(src ${Textures})
	STRING(REPLACE ${dataPath} ${OUT_TEXTURE_DIR} DstFile ${src})
	#message("Texture: " ${src} " to " ${DstFile})
	add_custom_command(
		OUTPUT ${DstFile}
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${src} ${DstFile}
		DEPENDS ${src} )
	list(APPEND DstFiles ${DstFile})
endforeach()
SET(DATA_DEP ${DATA_DEP} ${DstFiles})

if (EXISTS ${dataPath}/comp)
	add_custom_command(
		OUTPUT ${OUT_TEXTURE_DIR}/comp
		COMMAND ${CMAKE_COMMAND} -E copy_directory  ${dataPath}/comp ${OUT_TEXTURE_DIR}/comp
		DEPENDS ${src} )
	SET(DATA_DEP ${DATA_DEP} ${OUT_TEXTURE_DIR}/comp)
endif()

