/**
 * Copyright (c) 2017, Lukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 */

#ifndef _ATLIB_STREAM_GUARD_H_
#define _ATLIB_STREAM_GUARD_H_

#include <stdint.h>

namespace at {
    class Stream {
    public:
        Stream(void) {}

        virtual void write(uint8_t data);

        virtual uint8_t read(void);

        void print(const char *data) {
            while (*data)
                write(*(data++));
        }

        void print(int value) {
            char buff[10] = {0};
            char *p = buff + 8;
            if (value < 0) {
                write('-');
                value = -value;
            }
            do {
                *(p--) = (value % 10) + '0';
                value /= 10;
            } while (value);
            print((const char *) (p + 1));
        }

        void println(const char *data) {
            print(data);
            print("\r\n");
        }
    }; /* End of class Stream */
} /* End of namespace at */

#endif