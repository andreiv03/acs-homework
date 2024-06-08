CC = gcc
CFLAGS = -Wall -g

build: main

main: main.o operations.o
	$(CC) -o main $^ 

main.o: main.c
	$(CC) $(CFLAGS) -c $^

operations.o: operations.c
	$(CC) $(CFLAGS) -c $^

run: build
	./main

check: build
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./main

clean:
	rm -f *.o main
