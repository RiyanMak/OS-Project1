CC = gcc
CFLAGS = -Wall -Wextra

all: logger encryption driver

logger: main.c logger.c
	$(CC) $(CFLAGS) main.c logger.c -o logger

encryption: encryption.c
	$(CC) $(CFLAGS) encryption.c -o encryption

driver: driver.c
	$(CC) $(CFLAGS) driver.c -o driver

clean:
	rm -f logger encryption driver
