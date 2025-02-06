#include <string.h>
#include <errno.h>

#include <elog.h>

#include "ush.h"
#include "cmdline.h"
#include "history.h"
// #include "char.h"
// #include "term.h"


#undef UAIO_ARG1
#undef UAIO_ARG2
#undef UAIO_ENTITY
#define UAIO_ENTITY ush
#include "uaio_generic.c"


#define NEED_ANY 0
#define NEED_NOTHING -1


static int
_init(struct ush *sh) {
    struct ush_history *h = NULL;
    struct ush_cmdline *c = NULL;

    memset(&sh->reader, 0, sizeof(struct euart_reader));
    sh->reader.buff = malloc(CONFIG_USH_READER_CHUNKSIZE + 1);
    if (sh->reader.buff == NULL) {
        return -1;
    }
    sh->reader.timeout_us = 0;
    sh->reader.max = CONFIG_USH_CMDLINE_MAXCHARS;
    sh->reader.device = &sh->console;

    h = history_create(CONFIG_USH_HISTORY_MASKBITS);
    if (h == NULL) {
        goto failed;
    }
    sh->history = h;

    c = cmdline_create(CONFIG_USH_CMDLINE_MAXCHARS);
    if (c == NULL) {
        goto failed;
    }
    sh->cmdline = c;

    return 0;

failed:
    if (sh->reader.buff) {
        free(sh->reader.buff);
    }

    if (h) {
        free(h);
    }

    if (c) {
        free(c);
    }

    return -1;
}


static int
_deinit(struct ush *sh) {
    int ret = 0;

    if (sh == NULL) {
        return -1;
    }

    ret |= history_dispose(sh->history);
    ret |= cmdline_dispose(sh->cmdline);
    return ret;
}


ASYNC
ushA(struct uaio_task *self, struct ush *sh) {
    struct euart_reader *r = &sh->reader;
    UAIO_BEGIN(self);

    /* initialization */
    if (_init(sh)) {
        UAIO_THROW2(self, ENOMEM);
    }

    /* loop */
    while (true) {
        DEBUG("Reading %d bytes", r->max);
        r->bytes = 0;
        EUART_AREAD(self, r);
        DEBUG("STDIN(%d): %.*s", r->bytes, r->bytes, r->buff);
    }

    /* termination */
    UAIO_FINALLY(self);
    if (_deinit(sh)) {
        ERROR("_deinit");
    }

// prompt:
//     printf("\033[0m");
//     printf("root@esp:~# ");
//     while (true) {
//         fflush(stdout);
//         EUART_AWAIT(self, euart_getcA, &sh->console, &_read);
//         if (_read.status != EUCS_OK) {
//             continue;
//         }
//
//         sh->currentchar = _read.c;
//
//         // /* debug */
//         // DEBUG("%d %c", sh->currentchar, sh->currentchar);
//         // continue;
//
//         /* ansi and arrow keys */
//         if (ISNAV(sh->currentchar)) {
//             UAIO_AWAIT(self, ush, _navA, sh);
//             continue;
//         }
//
//         /* enter */
//         if (sh->currentchar == CHAR_LF) {
//             term_navend(sh);
//             putchar('\n');
//             sh->historyoffset = 0;
//             if (sh->cmdsize) {
//                 sh->cmdline[sh->cmdsize] = '\0';
//                 history_updatecurrent(sh);
//                 UAIO_AWAIT(self, ush, _execA, sh);
//                 sh->cmdsize = 0;
//                 history_alloc(sh);
//             }
//             sh->cursor = 0;
//             goto prompt;
//         }
//
//         /* insert char */
//         if (term_insert(sh, sh->currentchar)) {
//             UAIO_THROW(self, ENOMEM);
//         }
//     }
}
