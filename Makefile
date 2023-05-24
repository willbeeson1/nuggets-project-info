# Makefile for the client directory

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../support -I../maps 
LIBS = -lncurses ../support/support.a

# Executable names
CLIENT = client
TEST_CLIENT = test_client
LEGALMOVE = legalmove

# Object files
CLIENT_OBJS = client.o
TEST_CLIENT_OBJS = test_client.o
LEGALMOVE_OBJS = legalmove.o

# Phony targets
.PHONY: all clean test

all: $(CLIENT) $(TEST_CLIENT) $(LEGALMOVE)

$(CLIENT): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

$(TEST_CLIENT): $(TEST_CLIENT_OBJS) ../support/file.o 
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

$(LEGALMOVE): $(LEGALMOVE_OBJS) ../support/file.o 
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

client.o: client.c
	$(CC) $(CFLAGS) -c $< -o $@

test_client.o: test_client.c client.h
	$(CC) $(CFLAGS) -c $< -o $@

legalmove.o: legalmove.c client.h ../support/file.h
	$(CC) $(CFLAGS) -c $< -o $@

../support/file.o: ../support/file.c ../support/file.h
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_CLIENT)
	./$(TEST_CLIENT) ../maps/main.txt

legal: $(LEGALMOVE)
	./$(LEGALMOVE) ../maps/main.txt

clean:
	rm -f $(CLIENT_OBJS) $(TEST_CLIENT_OBJS) $(LEGALMOVE_OBJS) $(CLIENT) $(TEST_CLIENT) $(LEGALMOVE) ../support/file.o
