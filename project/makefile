CC = gcc
CFLAGS = -I./include -Wall -Wextra -g -ansi -pedantic -std=c99


SRCS=$(wildcard *.c)
OBJS = $(SRCS:.c=.o)


TARGET = assembly_program


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^


run: $(TARGET)
	./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all  ./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) 