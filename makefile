CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra
SRC_DIR=src
TEST_DIR=test
INC_DIR=include
LDFLAGS=-lm


SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJS=$(patsubst $(SRC_DIR)/%.c,%.o,$(SRCS))
TEST_SRCS=$(TEST_DIR)/tests.c
TARGET=tests

.PHONY: all clean run

all: $(TARGET)

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c -o $@ $<

$(TARGET): $(OBJS) $(TEST_SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

#make to compile the program
#make run to run the program
#make clean to clean the program