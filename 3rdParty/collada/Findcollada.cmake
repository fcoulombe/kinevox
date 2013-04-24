IF(${WIN32})
SET(COLLADA_ROOT "${CMAKE_CURRENT_LIST_DIR}")

SET(COLLADA_INCLUDE_DIR ${COLLADA_ROOT}/include/COLLADABaseUtils)
SET(COLLADA_INCLUDE_DIR ${COLLADA_INCLUDE_DIR} ${COLLADA_ROOT}/include/COLLADAFramework)
SET(COLLADA_INCLUDE_DIR ${COLLADA_INCLUDE_DIR} ${COLLADA_ROOT}/include/COLLADASaxFrameworkLoader)


SET(COLLADA_DEBUG_LIBRARY debug ${COLLADA_ROOT}/lib/${KINEVOX_ARCHITECTURE}/COLLADABaseUtils_d.lib)
SET(COLLADA_DEBUG_LIBRARY ${COLLADA_DEBUG_LIBRARY} debug ${COLLADA_ROOT}/lib/${KINEVOX_ARCHITECTURE}/COLLADAFramework_d.lib)
SET(COLLADA_DEBUG_LIBRARY ${COLLADA_DEBUG_LIBRARY} debug ${COLLADA_ROOT}/lib/${KINEVOX_ARCHITECTURE}/COLLADASaxFrameworkLoader_d.lib)
SET(COLLADA_DEBUG_LIBRARY ${COLLADA_DEBUG_LIBRARY} debug ${COLLADA_ROOT}/lib/${KINEVOX_ARCHITECTURE}/GeneratedSaxParser_d.lib)
SET(COLLADA_DEBUG_LIBRARY ${COLLADA_DEBUG_LIBRARY} debug ${COLLADA_ROOT}/lib/${KINEVOX_ARCHITECTURE}/LibXML_d.lib)
SET(COLLADA_DEBUG_LIBRARY ${COLLADA_DEBUG_LIBRARY} debug ${COLLADA_ROOT}/lib/${KINEVOX_ARCHITECTURE}/MathMLSolver_d.lib)
SET(COLLADA_DEBUG_LIBRARY ${COLLADA_DEBUG_LIBRARY} debug ${COLLADA_ROOT}/lib/${KINEVOX_ARCHITECTURE}/pcre_d.lib)

SET(COLLADA_LIBRARY optimized ${COLLADA_ROOT}/lib/${KINEVOX_ARCHITECTURE}/COLLADABaseUtils.lib)
SET(COLLADA_LIBRARY ${COLLADA_LIBRARY} optimized ${COLLADA_ROOT}/lib/${KINEVOX_ARCHITECTURE}/COLLADAFramework.lib)
SET(COLLADA_LIBRARY ${COLLADA_LIBRARY} optimized ${COLLADA_ROOT}/lib/${KINEVOX_ARCHITECTURE}/COLLADASaxFrameworkLoader.lib)
SET(COLLADA_LIBRARY ${COLLADA_LIBRARY} optimized ${COLLADA_ROOT}/lib/${KINEVOX_ARCHITECTURE}/GeneratedSaxParser.lib)
SET(COLLADA_LIBRARY ${COLLADA_LIBRARY} optimized ${COLLADA_ROOT}/lib/${KINEVOX_ARCHITECTURE}/LibXML.lib)
SET(COLLADA_LIBRARY ${COLLADA_LIBRARY} optimized ${COLLADA_ROOT}/lib/${KINEVOX_ARCHITECTURE}/MathMLSolver.lib)
SET(COLLADA_LIBRARY ${COLLADA_LIBRARY} optimized ${COLLADA_ROOT}/lib/${KINEVOX_ARCHITECTURE}/pcre.lib)

SET(collada_INCLUDE_DIR ${COLLADA_INCLUDE_DIR} )
list(APPEND collada_LIBS ${COLLADA_LIBRARY} )
list(APPEND collada_LIBS ${COLLADA_DEBUG_LIBRARY} )

#SET(collada_LIBS ${COLLADA_LIBRARY} ${COLLADA_DEBUG_LIBRARY} )
ELSE()


FIND_PACKAGE(LibXml2)

SET(COLLADA_ROOT "${CMAKE_CURRENT_LIST_DIR}../opencollada_release/")
 find_path(COLLADA_BASE_INCLUDE_DIR COLLADABU.h
  ${COLLADA_ROOT}/include/COLLADABaseUtils
  /usr/local/include/opencollada/COLLADABaseUtils
  )
  find_path(COLLADA_FRAMEWORK_INCLUDE_DIR COLLADAFW.h
  ${COLLADA_ROOT}/include/COLLADAFramework
  /usr/local/include/opencollada/COLLADAFramework
  )
  find_path(COLLADA_SAXFRAMEWORKLOADER_INCLUDE_DIR COLLADASaxFWLLoader.h
  ${COLLADA_ROOT}/include/COLLADASaxFrameworkLoader
  /usr/local/include/opencollada/COLLADASaxFrameworkLoader
  )

 SET(collada_INCLUDE_DIR ${COLLADA_BASE_INCLUDE_DIR} ${COLLADA_FRAMEWORK_INCLUDE_DIR} ${COLLADA_SAXFRAMEWORKLOADER_INCLUDE_DIR} )
 
 find_library(COLLADA_LIBRARY_COLLADABaseUtils 
                NAMES "OpenCOLLADABaseUtils" 
                HINTS ${COLLADA_ROOT}/lib/${KINEVOX_ARCHITECTURE}
                HINTS "/usr/local/lib/opencollada")
 find_library(COLLADA_LIBRARY_COLLADAFramework
                NAMES "OpenCOLLADAFramework" 
                HINTS ${COLLADA_ROOT}/lib/${KINEVOX_ARCHITECTURE}
                HINTS "/usr/local/lib/opencollada")
 find_library(COLLADA_LIBRARY_COLLADASaxFrameworkLoader 
                NAMES "OpenCOLLADASaxFrameworkLoader" 
                HINTS ${COLLADA_ROOT}/lib/${KINEVOX_ARCHITECTURE}
                HINTS "/usr/local/lib/opencollada")
 find_library(COLLADA_LIBRARY_GeneratedSaxParser 
                NAMES "GeneratedSaxParser" 
                HINTS ${COLLADA_ROOT}/lib/${KINEVOX_ARCHITECTURE}
                HINTS "/usr/local/lib/opencollada")
 find_library(COLLADA_LIBRARY_MathMLSolver 
                NAMES "MathMLSolver" 
                HINTS ${COLLADA_ROOT}/lib/${KINEVOX_ARCHITECTURE}
                HINTS "/usr/local/lib/opencollada")
 find_library(COLLADA_LIBRARY_UTF 
                NAMES "UTF" 
                HINTS ${COLLADA_ROOT}/lib/${KINEVOX_ARCHITECTURE}
                HINTS "/usr/local/lib/opencollada")
list(APPEND collada_LIBS ${COLLADA_LIBRARY_UTF})
list(APPEND collada_LIBS ${COLLADA_LIBRARY_COLLADABaseUtils})
list(APPEND collada_LIBS ${COLLADA_LIBRARY_COLLADAFramework})

list(APPEND collada_LIBS "pcre")
list(APPEND collada_LIBS ${LIBXML2_LIBRARIES})
list(APPEND collada_LIBS ${COLLADA_LIBRARY_GeneratedSaxParser})
list(APPEND collada_LIBS ${COLLADA_LIBRARY_MathMLSolver})

list(APPEND collada_LIBS ${COLLADA_LIBRARY_COLLADASaxFrameworkLoader})


#SET(collada_INCLUDE_DIR ${collada_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR})

#pcre
#libxml
ENDIF()
