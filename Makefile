build: *.c *.h
	gcc -Wall -g -o maze -g *.c *.h -lncurses

run: build
	./maze

clean:
	rm -f maze

mem-check:
	valgrind --leak-check=full --track-origins=yes ./maze