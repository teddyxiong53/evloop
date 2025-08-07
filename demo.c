// demo.c
#include "evloop.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void stdin_cb(int fd, short events, void *userdata) {
    char buf[1024];
    int n = read(fd, buf, sizeof(buf) - 1);
    if (n > 0) {
        buf[n] = 0;
        printf("Input: %s", buf);
        if (buf[0] == 'q') {
            ev_loop_stop((ev_loop *)userdata);
        }
    }
}

int main() {
    ev_loop *loop = ev_loop_new();

    ev_io stdin_watcher = {
        .fd = STDIN_FILENO,
        .events = EV_READ,
        .cb = stdin_cb,
        .userdata = loop,
    };

    ev_loop_add(loop, &stdin_watcher);
    printf("Type something (type 'q' to quit):\n");
    ev_loop_run(loop);
    ev_loop_destroy(loop);
    return 0;
}
