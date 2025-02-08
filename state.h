#ifndef STATE_H_
#define STATE_H_


#include <euart.h>

#include "ush.h"


struct console {
    struct euart_reader reader;
};


struct history {
    int head;
    int tail;
    int offset;
    int mask;
    unsigned int max;
    char *commands[];
};


typedef struct ush {
    struct console console;
    struct history history;

    /* user provided commands vector */
    struct ush_command *commands;
} ush_t;


#endif
