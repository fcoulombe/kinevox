SET(OUT_MUSIC_DIR ${CMAKE_CURRENT_BINARY_DIR}/data/Music/)
file(MAKE_DIRECTORY ${OUT_MUSIC_DIR})
file(GLOB musicFiles ${dataPath}*.mp3)
SET(LAME_EXE ${PROJECT_SOURCE_DIR}/3rdParty/lame/lame.exe)

SET(OGGENC_EXE ${PROJECT_SOURCE_DIR}/3rdParty/ogg/bin/oggenc.exe)

foreach(src ${musicFiles})
	get_filename_component(SrcExt ${src} EXT)
	string(REPLACE ${SrcExt} ".ogg" DstFile ${src})
	string(REPLACE ${dataPath} ${OUT_MUSIC_DIR} DstFile ${DstFile})
	#message(${LAME_EXE} --decode ${src} | ${OGGENC_EXE} -o ${DstFile})
	IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
		add_custom_command(
			OUTPUT ${DstFile}
			COMMAND ${LAME_EXE} --decode  ${src} ${DstFile}.wav
			COMMAND  ${OGGENC_EXE}  -o  ${DstFile} ${DstFile}.wav
			COMMAND ${CMAKE_COMMAND} -E remove ${DstFile}.wav
			DEPENDS ${SrcFile} )
	ELSE()
		add_custom_command(
			OUTPUT ${DstFile}
			COMMAND mpg321 ${src} -w - | oggenc -o ${DstFile} -
			DEPENDS ${SrcFile} 
			VERBATIM)
	ENDIF()

	list(APPEND DstFiles ${DstFile})
endforeach()
SET(DATA_DEP ${DATA_DEP} ${DstFiles})
#add_custom_target(MeshBuilder  DEPENDS ${DstFiles})
