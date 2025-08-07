// demo_eventfd.c
#include "evloop.h"
#include <sys/eventfd.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

int efd;

void event_cb(int fd, short events, void *userdata) {
    uint64_t val;
    read(fd, &val, sizeof(val));
    printf("[EVENTFD] Received signal (%llu)\n", (unsigned long long)val);
}

void *worker(void *arg) {
    sleep(2);
    uint64_t val = 1;
    write(efd, &val, sizeof(val));
    return NULL;
}

int main() {
    efd = eventfd(0, 0);

    ev_loop *loop = ev_loop_new();
    ev_io watcher = {
        .fd = efd,
        .events = EV_READ,
        .cb = event_cb,
    };

    ev_loop_add(loop, &watcher);

    pthread_t t;
    pthread_create(&t, NULL, worker, NULL);

    ev_loop_run(loop);
    ev_loop_destroy(loop);
    return 0;
}
