CC = gcc
CFLAGS = -Wall -g
RM = rm -f

default: all

all: p1

p1: p1.c
	$(CC) $(CFLAGS) -o p1 p1.c

clean veryclean:
	$(RM) p1


