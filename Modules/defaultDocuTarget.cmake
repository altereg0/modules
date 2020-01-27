MACRO(CONFIGURE_DOXYGEN_FILE DOXYGEN_CONFIG_FILE FILE_NAME_SUFFIX)
   IF(EXISTS ${PROJECT_SOURCE_DIR}/${DOXYGEN_CONFIG_FILE})
      FILE(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/doxy-${FILE_NAME_SUFFIX}.conf)
      FILE(READ ${PROJECT_SOURCE_DIR}/${DOXYGEN_CONFIG_FILE} DOXYFILE_CONTENTS)
      STRING(REGEX REPLACE ";" "\\\\;" DOXYFILE_CONTENTS "${DOXYFILE_CONTENTS}")
      STRING(REGEX REPLACE "\n" ";" DOXYFILE_LINES "${DOXYFILE_CONTENTS}")
      LIST(LENGTH DOXYFILE_LINES ROW)
      MATH(EXPR ROW "${ROW} - 1")
      FOREACH(I RANGE ${ROW})
         LIST(GET DOXYFILE_LINES ${I} LINE)
         IF(LINE STRGREATER "")
            STRING(REGEX MATCH "^[a-zA-Z]([^ ])+" DOXY_PARAM ${LINE})
            IF(DEFINED DOXY_${DOXY_PARAM})
               STRING(REGEX REPLACE "=([^\n])+" "= ${DOXY_${DOXY_PARAM}}" LINE ${LINE})
            ENDIF(DEFINED DOXY_${DOXY_PARAM})
         ENDIF()
         FILE(APPEND ${CMAKE_CURRENT_BINARY_DIR}/doxy-${FILE_NAME_SUFFIX}.conf "${LINE}\n")
      ENDFOREACH()
   ELSE()
      MESSAGE(SEND_ERROR "Doxygen configuration file '${DOXYGEN_CONFIG_FILE}' not found. Documentation will not be generated")
   ENDIF()
ENDMACRO(CONFIGURE_DOXYGEN_FILE)

MACRO(ADD_DOCUMENTATION TARGET DOXYGEN_CONFIG_FILE)
   FIND_PACKAGE(Doxygen)
   IF(DOXYGEN_FOUND)
      CONFIGURE_DOXYGEN_FILE(${DOXYGEN_CONFIG_FILE} ${TARGET})
      ADD_CUSTOM_TARGET(${TARGET} COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxy-${TARGET}.conf)
   ELSE(DOXYGEN_FOUND)
      MESSAGE(STATUS "Doxygen not found. Documentation will not be generated")
   ENDIF(DOXYGEN_FOUND)
ENDMACRO(ADD_DOCUMENTATION)

##################################################################################
# Default setup for a "docu" target using doxygen.
##################################################################################

##################################################################################
# find doxygen
##################################################################################
find_package(Doxygen)

##################################################################################
# create docu target, if package was found
##################################################################################
if(DOXYGEN_FOUND)
   if(NOT DOXYGEN_CONF_IN)
      # find_file does not work as expected (WinXP, cmake 2.8.10-2.8.12)
      find_program(conf_file NAMES "doxygen.conf.in" PATHS ${CMAKE_CURRENT_SOURCE_DIR})
   else(NOT DOXYGEN_CONF_IN)
      set(conf_file ${DOXYGEN_CONF_IN})
   endif(NOT DOXYGEN_CONF_IN)

   if(NOT conf_file)
      message(WARNING "No doxygen configuration found. Please create
                       '${CMAKE_CURRENT_SOURCE_DIR}/doxygen.conf.in'.
                       No documentation target created.")
   else(NOT conf_file)

      SET(DOXY_OUTPUT_LANGUAGE "Russian")
      SET(DOXY_INPUT ${PROJECT_SOURCE_DIR})
      ADD_DOCUMENTATION(doc Doxyfile)


   endif(NOT conf_file)
else(DOXYGEN_FOUND)
   message(WARNING "Doxygen not found. Documentation target not created.")
endif(DOXYGEN_FOUND)


