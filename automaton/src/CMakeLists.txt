#####################################################################
# Automaton library
#####################################################################
add_compile_definitions( ATM_PLAYER_DISABLE_TONE )

set( AUTOMATON_SRC_FILES
        Atm_analog.cpp
        Atm_bit.cpp
        Atm_button.cpp
        Atm_command.cpp
        Atm_comparator.cpp
        atm_connector.cpp
        Atm_controller.cpp
        atm_counter.cpp
        Atm_digital.cpp
        Atm_encoder.cpp
        Atm_fade.cpp
        Atm_fan.cpp
        Atm_led.cpp
        Atm_player.cpp
        Atm_step.cpp
        Atm_timer.cpp
        atm_timer_millis.cpp
        Automaton.cpp
        Machine.cpp
        WMath.cpp
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
        ${CMAKE_CURRENT_SOURCE_DIR}
)

avr_target_link_libraries(
        automaton
        avrtools
)