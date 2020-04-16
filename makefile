# Makefile for sample sender/receiver UDP multicast programs

CC=gcc
CGLAGS = -g -Wall

all: multicast 

multicast: send.c recv.c
	$(CC) $(CFLAGS) -o send send.c
	$(CC) $(CFLAGS) -o recv recv.c

clean:
	rm send 
	rm recv
