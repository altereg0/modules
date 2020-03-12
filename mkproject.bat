SET MODULES_PATH="c:\avr8-gnu-toolchain-win32_x86\modules"

mklink /J .cmake %MODULES_PATH%\.cmake
mklink /J avrtools %MODULES_PATH%\..\AVRTools\AVRTools
mklink /J automaton %MODULES_PATH%\automaton\src
mklink /J tux %MODULES_PATH%\tux
mklink /J avrlib %MODULES_PATH%\avrlib
mklink /J fastarduino %MODULES_PATH%\fastduino

REM echo %MODULES_PATH%\.cmake

