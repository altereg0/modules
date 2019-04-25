#####################################################################
# Protothreads library
#####################################################################
add_avr_interface_library( pt )
avr_target_include_common_directories(
        pt INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)