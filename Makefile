FILES = main.c menu.c maze.c
HEADERS = include/menu.h include/maze.h

build: 
	gcc -Wall -g -o maze ${FILES} ${HEADERS} -lncurses

run: build
	./maze

clean:
	rm -f maze

check:
	valgrind --leak-check=full --track-origins=yes ./maze