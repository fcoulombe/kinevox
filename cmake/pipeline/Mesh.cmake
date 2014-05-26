SET(OUT_MESH_DIR ${CMAKE_CURRENT_BINARY_DIR}/data/Mesh/)
file(MAKE_DIRECTORY ${OUT_MESH_DIR})
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/Material/)
file(GLOB meshFiles ${dataPath}*.dae ${dataPath}*.fbx)
foreach(src ${meshFiles})
	get_filename_component(SrcExt ${src} EXT)
	string(REPLACE ${SrcExt} ".mesh" DstFile ${src})
	string(REPLACE ${dataPath} ${OUT_MESH_DIR} DstFile ${DstFile})

	add_custom_command(
		OUTPUT ${DstFile}
		COMMAND meshconverter ${src} ${DstFile}
		DEPENDS ${SrcFile} meshconverter)
	list(APPEND DstFiles ${DstFile})
endforeach()
SET(DATA_DEP ${DATA_DEP} ${DstFiles})
#add_custom_target(MeshBuilder  DEPENDS ${DstFiles})
