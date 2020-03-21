/*
 * Automaton.h - Reactive State Machine Framework for Arduino.
 * Published under the MIT License (MIT), Copyright (c) 2015-2016, J.P. van der Landen
 */

#pragma once
#include <stdio.h>

class atm_serial_debug {  // It seems necessary to put this code in .h to keep it from being compiled in unnecessarily
 public:
  static void trace(UNUSED Stream *stream,
                    Machine &machine,
                    const char label[],
                    const char current[],
                    const char next[],
                    const char trigger[],
                    uint32_t runtime,
                    uint32_t cycles) {
#ifndef FPRINT_SERIAL_DEBUG
    char buf[13];
    sprintf(buf, "%011u\t", static_cast<unsigned int>(time::millis()));
    *stream << buf << F(" >>> ") << label << F("@") << (&machine);
    stream->flush();
    *stream << F(" from ") << current << F(" to ") << (next);
    stream->flush();
    *stream << F(" on ") << trigger << F(" (") << cycles << F(" cycles in ") << runtime << F(" ms)\n");
    stream->flush();
#else
  fprintf_P(stdout, PSTR("%012lu\t >>> %s@%lu from %s to %s on %s (%lu cycles in %lu ms)\n"), time::millis(), label, (&machine), current, next, trigger, cycles, runtime);
#endif
  }
};
