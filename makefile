CC = gcc
CFLAGS = -Wall -Wextra

TARGET = concat
SOURCES = src/concat.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

concat:
	rm -f $(TARGET) $(OBJECTS)

