
CC=gcc
CFLAGS=-lpthread -Wall


consumerproducer: concurr1.c mt19937ar.c
	$(CC) concurr1.c mt19937ar.c -o consumerproducer $(CFLAGS)

clean: 
	rm consumerproducer
