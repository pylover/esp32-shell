#include <string.h>

#include "console.h"


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
