#include <string.h>

#include "ansi.h"
#include "console.h"


#undef UAIO_ARG1
#undef UAIO_ARG2
#undef UAIO_ENTITY
#define UAIO_ENTITY console
#include "uaio_generic.c"


int
console_init(struct console *c, struct euart_device *device) {
    char *buff = NULL;

    if ((c == NULL) || (device == NULL)) {
        return -1;
    }

    buff = malloc(CONFIG_USH_READER_CHUNKSIZE + 1);
    if (buff == NULL) {
        return -1;
    }

    memset(&c->reader, 0, sizeof(struct euart_reader));
    c->reader.timeout_us = CONFIG_USH_READER_TIMEOUT_US;
    c->reader.device = device;
    c->reader.buff = buff;
    c->reader.minbytes = 1;
    c->reader.maxbytes = CONFIG_USH_READER_CHUNKSIZE;
    return 0;
}


int
console_deinit(struct console *c) {
    if (c == NULL) {
        return -1;
    }

    if (c->reader.buff) {
        free(c->reader.buff);
    }

    return 0;
}


int
console_printf(struct console *c, const char *restrict fmt, ...) {
    int ret;
    va_list args;

    va_start(args, fmt);
    ret = vdprintf(c->reader.device->outfd, fmt, args);
    va_end(args);
    return ret;
}


int
console_prompt(struct console *c) {
    return console_printf(c, "%s%s:# ",
            ANSI_CSI ANSI_RESET ANSI_MODE,
            CONFIG_USH_PROMPT
        );
}


ASYNC
console_readA(struct uaio_task *self, struct console *c) {
    UAIO_BEGIN(self);
    UAIO_AWAIT(self, euart_reader, euart_readA, &c->reader);
    UAIO_FINALLY(self);
}
