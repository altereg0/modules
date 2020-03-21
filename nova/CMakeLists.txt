#####################################################################
# simple AVR executable
#####################################################################

set(MYPROJECT_CORE_SRC_FILES
        demo.cpp
        )
add_avr_executable(
        ${PROJECT_NAME}
        ${MYPROJECT_CORE_SRC_FILES}
)

avr_target_compile_definitions(
		${PROJECT_NAME}
		USART_STREAM_ON
)

avr_target_compile_definitions(
		fast-arduino-lib
		BREADBOARD_ATMEGA328P
)

#####################################################################
# libraries to link to
#####################################################################
avr_target_link_libraries(
        ${PROJECT_NAME}
		fast-arduino-lib
 )