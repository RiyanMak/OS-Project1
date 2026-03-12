CC = gcc
CFLAGS = -Wall -Wextra

TARGET = enc
SRCS = main.c logger.c encryption.c

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
