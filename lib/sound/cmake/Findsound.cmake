SET(DepName "sound")
ProcessDependencies(${DepName})

get_filename_component(SOUND_ROOT ${CMAKE_CURRENT_LIST_DIR}/../ REALPATH)
SET(SOUND_INCLUDE_DIR ${SOUND_ROOT} )


SET(sound_INCLUDE_DIR ${SOUND_INCLUDE_DIR} ${DEP_INC} )
list(APPEND sound_LIBS sound)
list(APPEND sound_LIBS ${DEP_LIBS} )

#SET(sound_LIBS sound ${DEP_LIBS} )
