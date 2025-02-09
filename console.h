#ifndef CONSOLE_H_
#define CONSOLE_H_


#include "state.h"


#define CONSOLE_AREAD(task, state, min) \
    (state)->reader.minbytes = min; \
    (state)->reader.bytes = 0; \
    UAIO_AWAIT(task, console, console_readA, state)


#define CONSOLE_AREAD_MORE(task, state, more) \
    (state)->reader.minbytes += more; \
    UAIO_AWAIT(task, console, console_readA, state)


int
console_init(struct console *c, struct euart_device *device);


int
console_deinit(struct console *c);


int
console_printf(struct console *c, const char *restrict fmt, ...);


int
console_prompt(struct console *c);


ASYNC
console_readA(struct uaio_task *self, struct console *c);


#endif
