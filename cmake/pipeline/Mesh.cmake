SET(OUT_MESH_DIR ${CMAKE_CURRENT_BINARY_DIR}/data/Mesh/)
file(MAKE_DIRECTORY ${OUT_MESH_DIR})
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/Material/)
file(GLOB meshFiles ${dataPath}*.dae ${dataPath}*.fbx)

if (${WIN32})
    SET(DEBUG_EXE_PATH tools/src/tools-build/meshconverter/Debug/meshconverter)
    SET(RELEASE_EXE_PATH tools/src/tools-build/meshconverter/Release/meshconverter)
else()
    SET(DEBUG_EXE_PATH tools/src/tools-build/meshconverter/meshconverter)
    SET(RELEASE_EXE_PATH tools/src/tools-build/meshconverter/meshconverter)
endif()
foreach(src ${meshFiles})
	get_filename_component(SrcExt ${src} EXT)
	string(REPLACE ${SrcExt} ".mesh" DstFile ${src})
	string(REPLACE ${dataPath} ${OUT_MESH_DIR} DstFile ${DstFile})
	add_custom_command(
		OUTPUT ${DstFile}
		COMMAND $<$<CONFIG:debug>:${DEBUG_EXE_PATH}>$<$<CONFIG:release>:${RELEASE_EXE_PATH}> ${src} ${DstFile}
		DEPENDS ${src} tools
		WORKING_DIRECTORY
        ${PROJECT_BINARY_DIR}
		)
	list(APPEND DstFiles ${DstFile})
endforeach()
SET(DATA_DEP ${DATA_DEP} ${DstFiles})
#add_custom_target(MeshBuilder  DEPENDS ${DstFiles})
