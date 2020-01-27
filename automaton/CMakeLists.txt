#####################################################################
# Automaton library
#####################################################################
add_compile_definitions( ATM_PLAYER_DISABLE_TONE )

set( AUTOMATON_SRC_FILES
        src/Atm_analog.cpp
        src/Atm_bit.cpp
        src/Atm_button.cpp
        src/Atm_command.cpp
        src/Atm_comparator.cpp
        src/atm_connector.cpp
        src/Atm_controller.cpp
        src/atm_counter.cpp
        src/Atm_digital.cpp
        src/Atm_encoder.cpp
        src/Atm_fade.cpp
        src/Atm_fan.cpp
        src/Atm_led.cpp
        src/Atm_player.cpp
        src/Atm_step.cpp
        src/Atm_timer.cpp
        src/atm_timer_millis.cpp
        src/Automaton.cpp
        src/Machine.cpp
        src/WMath.cpp
        )

#target_compile_options(
#        automaton
#        ATM_PLAYER_DISABLE_TONE
#)

add_avr_library(
        automaton
        ${AUTOMATON_SRC_FILES}
)

avr_target_include_common_directories(
        automaton PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

avr_target_link_libraries(
        automaton
        avrtools
)