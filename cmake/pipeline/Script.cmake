SET(OUT_SOUND_DIR ${CMAKE_CURRENT_BINARY_DIR}/data/Script/)
file(MAKE_DIRECTORY ${OUT_SOUND_DIR})
file(GLOB confFiles ${dataPath}/*.lua)
foreach(src ${confFiles})
    STRING(REPLACE ${dataPath} ${OUT_SOUND_DIR} DstFile ${src})
    add_custom_command(
        OUTPUT ${DstFile}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${src} ${DstFile}
        DEPENDS ${src} )
    list(APPEND DstFiles ${DstFile})
endforeach()
SET(DATA_DEP ${DATA_DEP} ${DstFiles})

