
CC=gcc
CFLAGS=-lpthread -Wall

all: problem1 problem2

barber: barber.c 
	$(CC) barber.c -o barber $(CFLAGS)

problem2: cigarette.c
	$(CC) cigarette.c -o cig $(CFLAGS)

clean: 
	rm barber cig
