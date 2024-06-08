CFLAGS := -Wall -g -std=c99
FILES := lib/graph.c lib/priority-queue.c lib/task-one.c lib/task-two.c
HEADERS := include/graph.h include/priority-queue.h include/task-one.h include/task-two.h

all: build

build: main.c ${FILES} ${HEADERS}
	gcc ${CFLAGS} -o tema3 main.c ${FILES} ${HEADERS}

pack:
	zip -9 -FSr tema3.zip main.c ${FILES} ${HEADERS} Makefile README

clean:
	rm -rf tema3 tema3.zip