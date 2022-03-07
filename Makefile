CC=gcc
CFLAGS=-I. -Wall

client: client.c requests.c helpers.c buffer.c
	$(CC) -o client client.c requests.c helpers.c buffer.c

run: client
	./client

clean:
	rm -f *.o client
