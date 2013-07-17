SET(OUT_SCRIPT_DIR ${CMAKE_CURRENT_BINARY_DIR}/data/Archetype/)
file(MAKE_DIRECTORY ${OUT_SCRIPT_DIR})
file(GLOB scriptFiles ${dataPath}*.arch)
IF(${WIN32})
   	SET(LUACOMPILER_EXE ${PROJECT_SOURCE_DIR}/3rdParty/binaries/${CMAKE_SYSTEM_NAME}${CMAKE_CXX_COMPILER_ID}${KINEVOX_ARCHITECTURE}/usr/bin/luac.exe)
ELSE()
SET(LUACOMPILER_EXE luac)
ENDIF()
foreach(src ${scriptFiles})
	get_filename_component(SrcExt ${src} EXT)
	string(REPLACE ${SrcExt} ".arch" DstFile ${src})
	string(REPLACE ${dataPath} ${OUT_SCRIPT_DIR} DstFile ${DstFile})

	add_custom_command(
		OUTPUT ${DstFile}
		COMMAND ${LUACOMPILER_EXE} -o ${DstFile} ${src} 
		DEPENDS ${src} )
	list(APPEND DstFiles ${DstFile})
endforeach()
SET(DATA_DEP ${DATA_DEP} ${DstFiles})
#add_custom_target(MeshBuilder  DEPENDS ${DstFiles})
