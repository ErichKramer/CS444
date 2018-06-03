
CC=gcc
CFLAGS=-lpthread -Wall

all: barber cigarette

barber: barber.c 
	$(CC) barber.c -o barber $(CFLAGS)

cigaretete: cigarette.c
	$(CC) cigarette.c -o cig $(CFLAGS)

clean: 
	rm barber cig
