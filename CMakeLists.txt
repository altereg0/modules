##########################################################################
# "THE ANY BEVERAGE-WARE LICENSE" (Revision 42 - based on beer-ware
# license):
# <dev@layer128.net> wrote this file. As long as you retain this notice
# you can do whatever you want with this stuff. If we meet some day, and
# you think this stuff is worth it, you can buy me a be(ve)er(age) in
# return. (I don't like beer much.)
#
# Matthias Kleemann
##########################################################################

cmake_minimum_required(VERSION 3.14)

### TOOLCHAIN SETUP AREA #################################################
# Set any variables used in the toolchain prior project() call. In that
# case they are already set and used.
##########################################################################

##########################################################################
# tools to beused for programming the AVR
##########################################################################
set(AVR_UPLOADTOOL avrdude)
set(AVR_PROGRAMMER usbasp)
set(AVR_UPLOADTOOL_PORT usb)

##########################################################################
# AVR and fuses needs to be set
##########################################################################
set(AVR_MCU atmega2560)
set(AVR_MCU_SPEED "16000000UL")
set(AVR_H_FUSE 0xDF)
set(AVR_L_FUSE 0xFF)
set(AVR_E_FUSE 0xFC)
set(AVR_UPLOADTOOL_OPTIONS -B 0.4)

### END TOOLCHAIN SETUP AREA #############################################

include(.cmake/generic-gcc-avr.cmake)

##########################################################################
# name your project
##########################################################################
project(any)

##########################################################################
# status messages
##########################################################################
message(STATUS "Current uploadtool is: ${AVR_UPLOADTOOL}")
message(STATUS "Current programmer is: ${AVR_PROGRAMMER}")
message(STATUS "Current upload port is: ${AVR_UPLOADTOOL_PORT}")
message(STATUS "Current uploadtool options are: ${AVR_UPLOADTOOL_OPTIONS}")
message(STATUS "Current MCU is set to: ${AVR_MCU}")
message(STATUS "Current MCU speed is set to: ${AVR_MCU_SPEED}")
message(STATUS "Current H_FUSE is set to: ${AVR_H_FUSE}")
message(STATUS "Current L_FUSE is set to: ${AVR_L_FUSE}")

##########################################################################
# compiler options for all build types
##########################################################################
add_compile_definitions(
        F_CPU=${AVR_MCU_SPEED}
)
add_compile_options(
        -mmcu=${AVR_MCU}
        -c
        -Wall
        -Wundef
        -Werror
        -pedantic
        -pedantic-errors
        -fpack-struct
        -fshort-enums
        -funsigned-char
        -funsigned-bitfields
        -ffunction-sections
        -fdata-sections
        -mshort-calls
#        -v
)
#extra
#-mcall-prologues
#-mno-interrupts
#-nostdinc
#



##########################################################################
# include search paths
##########################################################################
#include_directories(c:/avr8/avrtools/AVRTOOLS)

##########################################################################
# building library and application in their subdirectories
##########################################################################
add_subdirectory(avrtools)
add_subdirectory(pt)
add_subdirectory(any)

add_subdirectory(any)

##########################################################################
# use default documentation target
##########################################################################
include("${PROJECT_SOURCE_DIR}/Modules/defaultDocuTarget.cmake")