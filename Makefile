
CC = gcc
CFLAGS = -Wall

all:
	$(CC) $(CFLAGS) -o bar status-pager.c config.h -lX11
