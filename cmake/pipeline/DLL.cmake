list(LENGTH DEP_DLL dllLen)
IF(${dllLen} GREATER 0)
	list(REMOVE_DUPLICATES DEP_DLL)
ENDIF()
SET(OUT_DLL_DIR ${CMAKE_CURRENT_BINARY_DIR})

foreach(src ${DEP_DLL})
	get_filename_component(fileName ${src} NAME)
	message("FLL: " ${fileName})
	
	SET(DstFile ${OUT_DLL_DIR}/${fileName})
	add_custom_command(
		OUTPUT ${DstFile}
		COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${src} ${DstFile}
		DEPENDS ${src} )
	list(APPEND DstFiles ${DstFile})
endforeach()
SET(DATA_DEP ${DATA_DEP} ${DstFiles})
