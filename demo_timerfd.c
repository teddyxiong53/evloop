// demo_timerfd.c
#include "evloop.h"
#include <sys/timerfd.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void timer_cb(int fd, short events, void *userdata) {
    uint64_t expirations;
    read(fd, &expirations, sizeof(expirations));
    printf("[TIMER] Tick! (%llu)\n", (unsigned long long)expirations);
}

int main() {
    int tfd = timerfd_create(CLOCK_MONOTONIC, 0);
    struct itimerspec spec = {
        .it_interval = {1, 0}, // 每秒一次
        .it_value = {1, 0},    // 1秒后第一次触发
    };
    timerfd_settime(tfd, 0, &spec, NULL);

    ev_loop *loop = ev_loop_new();
    ev_io watcher = {
        .fd = tfd,
        .events = EV_READ,
        .cb = timer_cb,
    };

    ev_loop_add(loop, &watcher);
    ev_loop_run(loop);
    ev_loop_destroy(loop);
    return 0;
}
