// demo_signalfd.c
#include "evloop.h"
#include <sys/signalfd.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

void signal_cb(int fd, short events, void *userdata) {
    struct signalfd_siginfo info;
    read(fd, &info, sizeof(info));
    printf("[SIGNALFD] Received signal: %d\n", info.ssi_signo);
    ev_loop_stop((ev_loop *)userdata);
}

int main() {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigprocmask(SIG_BLOCK, &mask, NULL);  // 必须阻塞原始信号

    int sfd = signalfd(-1, &mask, 0);

    ev_loop *loop = ev_loop_new();
    ev_io watcher = {
        .fd = sfd,
        .events = EV_READ,
        .cb = signal_cb,
        .userdata = loop,
    };

    ev_loop_add(loop, &watcher);
    printf("Press Ctrl+C to trigger SIGINT...\n");
    ev_loop_run(loop);
    ev_loop_destroy(loop);
    return 0;
}
