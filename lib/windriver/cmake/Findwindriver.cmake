SET(DepName "windriver")
ProcessDependencies(${DepName})
SET(windriver_ROOT ${CMAKE_CURRENT_LIST_DIR}/../)
SET(windriver_INCLUDE_DIR ${windriver_ROOT})


SET(windriver_INCLUDE_DIR ${windriver_INCLUDE_DIR} ${DEP_INC})
IF (${WIN32})
	list(APPEND windriver_LIBS "wsock32.lib" )
ELSEIF(${UNIX})
	list(APPEND windriver_LIBS "X11" )
ENDIF()
list(APPEND windriver_LIBS windriver)
list(APPEND windriver_LIBS ${DEP_LIBS} )


