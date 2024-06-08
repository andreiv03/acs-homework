FILES := lib/list.c lib/queue.c lib/stack.c lib/utils.c
HEADERS := include/list.h include/queue.h include/stack.h include/utils.h

build: main.c ${FILES} ${HEADERS}
	gcc -o tema1 -g main.c ${FILES} ${HEADERS}

run: build
	./tema1

pack:
	zip -9 -FSr tema1.zip main.c ${FILES} ${HEADERS} Makefile README

clean:
	rm -rf tema1 tema1.in tema1.out tema1.zip