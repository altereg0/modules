/*
 * This empty project serves as a blueprint for new projects using FastArduino library.
 * The project comes configured for all supported targets, but you may possibly
 * choose only one target in your project.
 * If you do use several targets it si likely that you will need to add conditional
 * compilation with `#ifdef` to set different constants (e.g. pins, ports) according
 * to each target you want to support, as shown below.
 */

// Include necessary fastarduino headers
//--------------------------------------
#include <fastarduino/boards/board.h>
#include <fastarduino/gpio.h>
#include <fastarduino/time.h>
#include <fastarduino/uart.h>

// Define specific constants per target
//-------------------------------------
// If you have only one target then you can remove all this section.
#if defined(ARDUINO_UNO) || defined(BREADBOARD_ATMEGA328P) || defined(ARDUINO_NANO)
#elif defined(ARDUINO_LEONARDO)
#elif defined (ARDUINO_MEGA)
#elif defined (BREADBOARD_ATTINYX4)
#else
#error "Current target is not yet supported!"
#endif

// Register all ISR here if any
//-----------------------------
// Define vectors we need in the example
REGISTER_UATX_ISR(0)

// Buffers for UART
static const uint8_t OUTPUT_BUFFER_SIZE = 32;
static char output_buffer[OUTPUT_BUFFER_SIZE];

// Main entry point
//-----------------

// Your main() function is the starting point of your program, it will be called
// as soon as the MCU is switched on or reset.
int main() __attribute__((OS_main));
int main() {
  // DO NOT REMOVE this line!
  board::init();

  // Add any specific initialization needed by your project before allowing interrupts
  // Do something

  // Enable interrupts
  sei();

  // Start UART
  serial::hard::UATX<board::USART::USART0> uart{output_buffer};
  uart.begin(9600);
  streams::ostream out = uart.out();

  out.write(F("Start\n"));
  // Event Loop
  while (true)
  {
    out.write(F("ABCDEFGHIJKLMNOPQRSTUVWXYZ\n"));
    out.flush();
    out.write(F("abcdefghijklmnopqrstuvwxyz\n"));
    out.flush();
    out << F("0123456789+-*/\n");
    out.flush();
    time::delay_ms(1000);
  }

  // Normally you should never reach the following line, but it is necessary to
  // make the compiler happy :-)
  return 0;
}
 