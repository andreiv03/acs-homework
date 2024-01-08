FILES := lib/game.c lib/list.c lib/queue.c lib/stack.c lib/tree.c lib/utils.c
HEADERS := include/game.h include/list.h include/queue.h include/stack.h include/tree.h include/utils.h

build: main.c ${FILES} ${HEADERS}
	gcc -o lanParty -Wall -g main.c ${FILES} ${HEADERS}

run: build
	./lanParty ./date/t14/c.in ./date/t14/d.in ./out/out14.out

clean:
	rm -f lanParty