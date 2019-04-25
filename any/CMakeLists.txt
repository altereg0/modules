#####################################################################
# simple AVR executable
#####################################################################

set(MYPROJECT_CORE_SRC_FILES
        demo.cpp
        )
add_avr_executable(
        any
        ${MYPROJECT_CORE_SRC_FILES}
)

#####################################################################
# libraries to link to
#####################################################################
avr_target_link_libraries(
        tiny
        avrtools
		tiny
        pt
 )