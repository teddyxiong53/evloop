CC = gcc
CFLAGS = -Wall -O2

all: demo

demo: demo.c evloop.c evloop.h
	$(CC) $(CFLAGS) -o demo demo.c evloop.c

clean:
	rm -f demo
