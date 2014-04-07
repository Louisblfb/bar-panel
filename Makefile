
CC = gcc
CFLAGS = -Wall

all:
	$(CC) $(CFLAGS) -o bar bar.c bar.h -lX11
