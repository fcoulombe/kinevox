SET(OUT_SCRIPT_DIR ${CMAKE_CURRENT_BINARY_DIR}/data/Script/)
file(MAKE_DIRECTORY ${OUT_SCRIPT_DIR})
file(GLOB scriptFiles ${dataPath}*.lua)
IF(${KINEVOX_ARCHITECTURE} MATCHES "32bit")
	SET(LUACOMPILER_EXE ${PROJECT_SOURCE_DIR}/3rdParty/lua/bin/luac.exe)
ELSE()
	SET(LUACOMPILER_EXE ${PROJECT_SOURCE_DIR}/3rdParty/lua/bin/luac64.exe)
ENDIF()
foreach(src ${scriptFiles})
	get_filename_component(SrcExt ${src} EXT)
	string(REPLACE ${SrcExt} ".luac" DstFile ${src})
	string(REPLACE ${dataPath} ${OUT_SCRIPT_DIR} DstFile ${DstFile})

	add_custom_command(
		OUTPUT ${DstFile}
		COMMAND ${LUACOMPILER_EXE} -o ${DstFile} ${src} 
		DEPENDS ${SrcFile} )
	list(APPEND DstFiles ${DstFile})
endforeach()
SET(DATA_DEP ${DATA_DEP} ${DstFiles})
#add_custom_target(MeshBuilder  DEPENDS ${DstFiles})
