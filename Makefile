build:
	gcc -o maze_game -g main.c -lncurses
run:
	./maze_game
clean:
	rm maze_game
mem-check:
	valgrind --leak-check=full --track-origins=yes maze_game