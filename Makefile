CC = gcc
CFLAGS = -Wall -Wextra

all: logger encryption

logger: main.c logger.c
	$(CC) $(CFLAGS) main.c logger.c -o logger

encryption: encryption.c
	$(CC) $(CFLAGS) encryption.c -o encryption

clean:
	rm -f logger encryption
