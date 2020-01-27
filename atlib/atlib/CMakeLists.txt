#####################################################################
# @lib library
#####################################################################

set(ATLIB_SRC_FILES
        src/io.cpp
        src/uart.cpp
        inc/at/stream.hpp

        )

#target_compile_options(
#        automaton
#        ATM_PLAYER_DISABLE_TONE
#)

add_avr_library(
        atlib
        ${ATLIB_SRC_FILES}
)

avr_target_include_common_directories(
        atlib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

message(STATUS "ATLIB: ${CMAKE_CURRENT_SOURCE_DIR}")
message(">\tCurrent build type: ${CMAKE_BUILD_TYPE}")
