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

#####################################################################
# libraries to link to
#####################################################################
avr_target_link_libraries(
        ${PROJECT_NAME}
        avrtools
 )