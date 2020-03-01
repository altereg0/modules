#####################################################################
# simple AVR executable
#####################################################################
set(TUX_CORE_SRC_FILES
        dhcp_client.c
        dnslkup.c
        enc28j60.c
        ip_arp_udp_tcp.c
        websrv_help_functions.c
        )

add_avr_library(tux-enc28j60 ${TUX_CORE_SRC_FILES})

avr_target_include_common_directories(
        tux-enc28j60 PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)