// evloop.h
#ifndef EVLOOP_H
#define EVLOOP_H

#include <poll.h>

#define EV_READ  POLLIN
#define EV_WRITE POLLOUT

typedef void (*event_callback)(int fd, short events, void *userdata);

typedef struct ev_io {
    int fd;
    short events;
    event_callback cb;
    void *userdata;
} ev_io;

typedef struct ev_loop ev_loop;

ev_loop *ev_loop_new(void);
void ev_loop_destroy(ev_loop *loop);
void ev_loop_add(ev_loop *loop, ev_io *watcher);
void ev_loop_remove(ev_loop *loop, int fd);
void ev_loop_run(ev_loop *loop);
void ev_loop_stop(ev_loop *loop);

#endif
