#ifndef CONSOLE_H_
#define CONSOLE_H_


#include "state.h"


int
console_init(struct console *c, struct euart_device *device);


int
console_deinit(struct console *c);


#endif
