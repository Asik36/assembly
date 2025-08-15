CC = gcc

CFLAGS = -Wall -Wextra -g -ansi -pedantic -std=c99
INCLUDES = -I./include/common_structs -Iinclude/format -Iinclude/machine_code -Iinclude/preassembly -Iinclude/sentence -Iinclude/symbol -Iinclude/memory_manager -Iinclude/output_files
CFLAGS+= $(INCLUDES)



SRC_DIRS = src/machine_code src/preassembly src/sentence src/symbol src/memory_manager src/output_files src/common
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