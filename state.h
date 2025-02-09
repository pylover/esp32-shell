#ifndef STATE_H_
#define STATE_H_


#include <euart.h>

#include "ush.h"


typedef struct console {
    struct euart_reader reader;
} console_t;


#undef UAIO_ARG1
#undef UAIO_ARG2
#undef UAIO_ENTITY
#define UAIO_ENTITY console
#include "uaio_generic.h"


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
