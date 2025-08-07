CC = gcc

CFLAGS = -Wall -Wextra -g -ansi -pedantic -std=c99
INCLUDES = -I./include -Isrc/format -Isrc/machine_code -Isrc/preassembly -Isrc/sentence -Isrc/symbol
CFLAGS+= $(INCLUDES)



SRC_DIRS = src/format src/machine_code src/preassembly src/sentence src/symbol	
SRCS=$(wildcard *.c)
OBJS = main.c $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))


TARGET = assembly_program


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^


run: $(TARGET)
	./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all  ./$(TARGET)

clean:
	rm -f $(TARGET) 