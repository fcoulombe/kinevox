SET(DepName "windriver")
ProcessDependencies(${DepName})
SET(windriver_ROOT ${CMAKE_CURRENT_LIST_DIR}/../)
SET(windriver_INCLUDE_DIR ${windriver_ROOT})


SET(windriver_INCLUDE_DIR ${windriver_INCLUDE_DIR} ${DEP_INC})
list(APPEND windriver_LIBS windriver)
list(APPEND windriver_LIBS ${DEP_LIBS} )

#SET(windriver_LIBS windriver ${DEP_LIBS} )
