#####################################################################
# tinyfsm library #
#####################################################################
#add_compile_definitions( TINYFSM_NOSTDLIB ) // requreid in parent_app
add_avr_interface_library( tiny )
avr_target_include_common_directories(
        tiny INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)