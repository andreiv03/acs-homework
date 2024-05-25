CC = g++
CCFLAGS = -Wall -Wextra -std=c++17 -O0 -lm

.PHONY: build clean

build: p1 p2 p3 p4

run-p1:
	./p1
run-p2:
	./p2
run-p3:
	./p3
run-p4:
	./p4

p1: numarare.cpp
	$(CC) -o $@ $^ $(CCFLAGS)
p2: trenuri.cpp
	$(CC) -o $@ $^ $(CCFLAGS)
p3: drumuri.cpp
	$(CC) -o $@ $^ $(CCFLAGS)
p4: scandal.cpp
	$(CC) -o $@ $^ $(CCFLAGS)

clean:
	rm -f p1 p2 p3 p4
