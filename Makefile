# Makefile for word command

CC = gcc
CFLAGS = -Wall
LDFLAGS = -lm -lposix
LIBS = -lbsd
TARGETS = word dir date 

all: $(TARGETS)

word: word.c
	$(CC) $(CFLAGS) -o word word.c
dir: dir.c
	$(CC) $(CFLAGS) -o dir dir.c
date: date.c
	$(CC) $(CFLAGS) -o date date.c $(LDFLAGS) $(LIBS)

clean:
	rm -f $(TARGETS) 
