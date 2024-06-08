CC = g++
CFLAGS = -Iinclude -Iinclude/commands -g -Wall -Wextra -std=c++17
SRCS = $(wildcard src/*.cpp src/commands/*.cpp)
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean

all: client

client: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o src/commands/*.o client

run: client
	./client
