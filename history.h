#ifndef HISTORY_H_
#define HISTORY_H_


struct ush_history {
    int head;
    int tail;
    int offset;
    int mask;
    unsigned int max;
    char *commands[];
};


struct ush_history *
history_create(unsigned int maskbits);


int
history_dispose(struct ush_history *h);


#endif  // HISTORY_H_
