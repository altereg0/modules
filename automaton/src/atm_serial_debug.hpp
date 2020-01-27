/*
 * Automaton.h - Reactive State Machine Framework for Arduino.
 * Published under the MIT License (MIT), Copyright (c) 2015-2016, J.P. van der Landen
 */

#pragma once

#include <SystemClock.h>
#include <Writer.h>

#include <stdio.h>
#include <avr/pgmspace.h>

class atm_serial_debug {  // It seems necessary to put this code in .h to keep it from being compiled in unnecessarily
 public:
  static void trace( Stream* stream, Machine& machine, const char label[], const char current[], const char next[], const char trigger[], uint32_t runtime,
                     uint32_t cycles ) {
    char buf[12];
    sprintf(buf, "%011lu", millis());
    stream->print(buf);
//    stream->print( '\t' );
//    stream->print( millis() );
    stream->print( " >>> " );
    stream->print( label );
    stream->print( "@" );
    stream->print( (long)&machine, Writer::IntegerOutputBase::kHex);
    stream->print( " from " );
    stream->print( current );
    stream->print( " to " );
    stream->print( next );
    stream->print( " on " );
    stream->print( trigger );
    stream->print( " (" );
    stream->print( cycles );
    stream->print( " cycles in " );
    stream->print( runtime );
    stream->println( " ms)" );
  }
};
