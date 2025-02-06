#ifndef USH_H_
#define USH_H_


#include <euart.h>
#include <uaio.h>


typedef struct ush_process {
    int argc;
    char **argv;
    void *userptr;
} ush_process_t;


#undef UAIO_ARG1
#undef UAIO_ARG2
#undef UAIO_ENTITY
#define UAIO_ENTITY ush_process
#include "uaio_generic.h"


struct ush_command {
    const char *name;
    ush_process_coro_t entrypoint;
};


typedef struct ush {
    /* console */
    struct euart_device console;
    struct euart_reader reader;

    struct ush_cmdline *cmdline;
    struct ush_history *history;

    /* user provided command vector */
    struct ush_command commands[];
} ush_t;


#undef UAIO_ARG1
#undef UAIO_ARG2
#undef UAIO_ENTITY
#define UAIO_ENTITY ush
#include "uaio_generic.h"


ASYNC
ushA(struct uaio_task *self, struct ush *sh);


#endif  // USH_H_
