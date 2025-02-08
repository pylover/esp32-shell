#include <string.h>
#include <errno.h>

#include <elog.h>

#include "ush.h"
#include "state.h"
#include "console.h"


#undef UAIO_ARG1
#undef UAIO_ARG2
#undef UAIO_ENTITY
#define UAIO_ENTITY ush
#include "uaio_generic.c"


struct ush *
ush_create(struct euart_device *terminal, struct ush_command commands[]) {
    struct ush *sh = NULL;

    if (terminal == NULL) {
        return NULL;
    }

    sh = malloc(sizeof(struct ush));
    if (sh == NULL) {
        return NULL;
    }

    if (console_init(&sh->console, terminal)) {
        goto failed;
    }

    sh->commands = commands;
    return sh;

failed:
    console_deinit(&sh->console);

    if (sh) {
        free(sh);
    }

    return NULL;
}


int
ush_destroy(struct ush *sh) {
    int ret = 0;

    if (sh == NULL) {
        return -1;
    }

    ret |= console_deinit(&sh->console);

    free(sh);
    return ret;
}


ASYNC
ushA(struct uaio_task *self, struct ush *sh) {
    UAIO_BEGIN(self);

    /* loop */
    DEBUG("linebreak: %d", CR[0]);
    // console_printf(&sh->console, CR);
    while (true) {
        DEBUG("linebreak: %d", CR[0]);
        // r->bytes = 0;
        // EUART_AREAD(self, r);
        // DEBUG("STDIN: %d", r->bytes);
    }

    /* termination */
    UAIO_FINALLY(self);

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
