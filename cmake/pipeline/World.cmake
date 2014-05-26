SET(OUT_WORLD_DIR ${CMAKE_CURRENT_BINARY_DIR}/data/World/)
file(MAKE_DIRECTORY ${OUT_WORLD_DIR})
file(GLOB worldFiles ${dataPath}*.dae ${dataPath}*.fbx)
foreach(src ${worldFiles})
    get_filename_component(SrcExt ${src} EXT)
    string(REPLACE ${SrcExt} ".world" DstFile ${src})
    string(REPLACE ${dataPath} ${OUT_WORLD_DIR} DstFile ${DstFile})

    add_custom_command(
        OUTPUT ${DstFile}
        COMMAND worldconverter ${src} ${DstFile}
        DEPENDS ${SrcFile} worldconverter)
    list(APPEND DstFiles ${DstFile})
endforeach()
SET(DATA_DEP ${DATA_DEP} ${DstFiles})