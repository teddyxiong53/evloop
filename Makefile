CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lpthread

all: demo_timerfd demo_eventfd demo_signalfd demo

demo: demo.c evloop.c evloop.h
	$(CC) $(CFLAGS) -o demo demo.c evloop.c

demo_timerfd: demo_timerfd.c evloop.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

demo_eventfd: demo_eventfd.c evloop.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

demo_signalfd: demo_signalfd.c evloop.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f demo_timerfd demo_eventfd demo_signalfd demo
