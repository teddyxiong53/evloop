// evloop.c
#include "evloop.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct ev_loop {
    struct pollfd *pollfds;
    ev_io **watchers;
    int count;
    int capacity;
    int running;
};

ev_loop *ev_loop_new(void) {
    ev_loop *loop = calloc(1, sizeof(ev_loop));
    loop->capacity = 16;
    loop->pollfds = calloc(loop->capacity, sizeof(struct pollfd));
    loop->watchers = calloc(loop->capacity, sizeof(ev_io *));
    return loop;
}

void ev_loop_destroy(ev_loop *loop) {
    free(loop->pollfds);
    free(loop->watchers);
    free(loop);
}

void ev_loop_add(ev_loop *loop, ev_io *watcher) {
    if (loop->count >= loop->capacity) {
        loop->capacity *= 2;
        loop->pollfds = realloc(loop->pollfds, loop->capacity * sizeof(struct pollfd));
        loop->watchers = realloc(loop->watchers, loop->capacity * sizeof(ev_io *));
    }
    loop->pollfds[loop->count].fd = watcher->fd;
    loop->pollfds[loop->count].events = watcher->events;
    loop->watchers[loop->count] = watcher;
    loop->count++;
}

void ev_loop_remove(ev_loop *loop, int fd) {
    for (int i = 0; i < loop->count; ++i) {
        if (loop->pollfds[i].fd == fd) {
            loop->pollfds[i] = loop->pollfds[loop->count - 1];
            loop->watchers[i] = loop->watchers[loop->count - 1];
            loop->count--;
            return;
        }
    }
}

void ev_loop_stop(ev_loop *loop) {
    loop->running = 0;
}

void ev_loop_run(ev_loop *loop) {
    loop->running = 1;
    while (loop->running) {
        int n = poll(loop->pollfds, loop->count, -1);
        if (n <= 0) continue;

        for (int i = 0; i < loop->count; ++i) {
            if (loop->pollfds[i].revents) {
                ev_io *watcher = loop->watchers[i];
                watcher->cb(watcher->fd, loop->pollfds[i].revents, watcher->userdata);
            }
        }
    }
}
