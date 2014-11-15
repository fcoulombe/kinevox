#
#recurse to find  .cmake files
#
macro(FindCMAKE path)
	file(GLOB_RECURSE CMAKE_MODS ${path}/*.cmake)
	foreach(p ${CMAKE_MODS})
		GET_FILENAME_COMPONENT(fn ${p} PATH)
		SET(pathList ${pathList} ${fn})
	endforeach()
	list(REMOVE_DUPLICATES pathList)
endmacro()

macro(FindCMakeLists path)
	file(GLOB_RECURSE CMAKE_MODS ${path}/CMakeLists.txt)
	foreach(p ${CMAKE_MODS})
		GET_FILENAME_COMPONENT(fn ${p} PATH)
		SET(cmakelist ${cmakelist} ${fn})
	endforeach()
	list(REMOVE_DUPLICATES cmakelist)
endmacro()


#
macro(ProcessDependencies projName)
	#message(${projName} ${DEP_LIBS})
	include (${projName}dep)
	SET(DebugOptimized "")
	foreach(dep ${DEPENDENCIES})
		FIND_PACKAGE( ${dep}  REQUIRED)
		SET(DEP_INC ${DEP_INC} ${${dep}_INCLUDE_DIR})
		SET(DEP_DLL ${DEP_DLL} ${${dep}_DLL})
		
		FOREACH( d ${${dep}_LIBS} )
			IF(${d} STREQUAL "debug" OR ${d} STREQUAL "optimized")
				SET(DebugOptimized ${d})
			ELSE()
				list(FIND DEP_LIBS ${d} isInList)
				IF(isInList EQUAL -1)
					IF(NOT ${DebugOptimized} STREQUAL "")
						list(APPEND DEP_LIBS ${DebugOptimized})
					ENDIF()
					list(APPEND DEP_LIBS ${d})
					#message("Adding: " ${d})
				ENDIF()
				SET(DebugOptimized "")
			ENDIF()
		ENDFOREACH()
		#SET(DEP_LIBS ${DEP_LIBS} ${${dep}_LIBS})
	endforeach()

	list(LENGTH DEP_INC listLen)
	if (${listLen} GREATER 1)
		list(REMOVE_DUPLICATES DEP_INC)
	endif()
endmacro()

SET(USE_NATIVE_ACTIVITY ON)
macro(AndroidExecutable ProjectName)
    add_library(${ProjectName} SHARED ${${ProjectName}_src} ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c ${DATA_DEP})
    set(ANDROID_SO_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/android/libs/${ANDROID_ABI})
    set_target_properties(${ProjectName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${ANDROID_SO_OUTDIR})
    set_target_properties(${ProjectName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${ANDROID_SO_OUTDIR})
    set_target_properties(${ProjectName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${ANDROID_SO_OUTDIR})
    IF(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
        list(REVERSE DEP_LIBS)
    ENDIF()
    SET(ANDROID_SDK_TOOL $ENV{ANDROID_SDK}/tools/android)
    SET(ANDROID_ANT ant)
    #SET(ANDROID_PLATFORM android-$(ANDROID_NATIVE_API_LEVEL))
    SET(ANDROID_PLATFORM android-20)
    execute_process(COMMAND ${ANDROID_SDK_TOOL} create project
             --path ${CMAKE_CURRENT_BINARY_DIR}/android
             --target ${ANDROID_PLATFORM}
             --name ${ProjectName}
             --package com.kinevox.${ProjectName}
             --activity KinevoxActivity
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
      set(ANT_BUILD_TYPE "debug")
      set(IS_DEBUGGABLE "true")
    else()
      set(IS_DEBUGGABLE "false")
    endif() 
    if (NOT ${USE_NATIVE_ACTIVITY})

        configure_file (
          "${PROJECT_SOURCE_DIR}/android/AndroidManifest_java.xml"
          "${CMAKE_CURRENT_BINARY_DIR}/android/AndroidManifest.xml"
          )
          
        configure_file (
          "${PROJECT_SOURCE_DIR}/android/KinevoxActivity.java"
          "${CMAKE_CURRENT_BINARY_DIR}/android/src/com/kinevox/${ProjectName}/KinevoxActivity.java"
          )
        configure_file (
          "${PROJECT_SOURCE_DIR}/android/KinevoxLib.java"
          "${CMAKE_CURRENT_BINARY_DIR}/android/src/com/kinevox/${ProjectName}/KinevoxLib.java"
          )
        configure_file (
          "${PROJECT_SOURCE_DIR}/android/KinevoxView.java"
          "${CMAKE_CURRENT_BINARY_DIR}/android/src/com/kinevox/${ProjectName}/KinevoxView.java"
          )
  
    else()
        configure_file (
          "${PROJECT_SOURCE_DIR}/android/AndroidManifest_native.xml"
          "${CMAKE_CURRENT_BINARY_DIR}/android/AndroidManifest.xml"
          )
    endif()  
    add_custom_command(TARGET ${ProjectName} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory assets WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/android)
    add_custom_command(TARGET ${ProjectName} POST_BUILD COMMAND ln -sf ../../data/ data WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/android/assets)        
    add_custom_command(TARGET ${ProjectName} POST_BUILD COMMAND ${ANDROID_ANT} -v ${ANT_BUILD_TYPE} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/android)          
    if (NOT "${DATA_DEP}" STREQUAL "")
        add_dependencies(${ProjectName} ${DATA_DEP})
    endif()
endmacro(AndroidExecutable ProjectName)

macro(Executable ProjectName)
	INCLUDE(DLL)
	if(${ANDROID})
	   SET(EXTRA_LIBS android log)
	   AndroidExecutable(${ProjectName})
	else()
	   SET(EXTRA_LIBS)
	   add_executable(${ProjectName} ${${ProjectName}_src} ${DATA_DEP})
	   IF(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
        list(REVERSE DEP_LIBS)
        list(APPEND DEP_LIBS "-lpthread")
       ENDIF()
	endif()

    target_link_libraries( ${ProjectName} ${DEP_LIBS} ${EXTRA_LIBS} )
endmacro()

macro(Test ProjectName)
	install (TARGETS ${ProjectName} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	add_test (NAME ${ProjectName}_run WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}  COMMAND ${ProjectName}  )
	set_tests_properties (${ProjectName}_run  PROPERTIES FAIL_REGULAR_EXPRESSION "FAILED")
	add_dependencies(build_and_test ${ProjectName})
    SET_TARGET_PROPERTIES( ${ProjectName} PROPERTIES FOLDER Test)
endmacro()


macro(ListSubdirectories curdir)
  file(GLOB sub-dir ${curdir}/*)
  set(list_of_dirs "")
  foreach(dir ${sub-dir})  	
    if(IS_DIRECTORY ${dir})
        set(list_of_dirs ${list_of_dirs} ${dir})
    endif()
  endforeach()
  
endmacro()

#
macro(ProcessData path)
	ListSubdirectories(${path})
	foreach(dataPath ${list_of_dirs})
		get_filename_component(pathName ${dataPath} NAME)
		SET(dataPath ${dataPath}/)
		Include(${pathName})
	endforeach()
endmacro()

macro(FindLibrary varname lib lpath)
	FIND_LIBRARY(${varname} NAMES ${lib}.a ${lib}.lib ${lib}.so ${lib}.dll  PATHS ${lpath} NO_DEFAULT_PATHS  NO_SYSTEM_ENVIRONMENT_PATH  )
endmacro()

