CFLAGS := -Wall -g -std=c99
FILES := lib/quadtree.c lib/queue.c lib/utils.c
HEADERS := include/quadtree.h include/queue.h include/utils.h

all: build

build: main.c ${FILES} ${HEADERS}
	gcc ${CFLAGS} -o quadtree main.c ${FILES} ${HEADERS} -lm

pack:
	zip -9 -FSr tema2.zip main.c ${FILES} ${HEADERS} Makefile README

clean:
	rm -rf quadtree tema2.zip
