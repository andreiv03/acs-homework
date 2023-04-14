CFLAGS := -Wall -g
FILES := lib/quadtree.c lib/queue.c lib/utils.c
HEADERS := include/quadtree.h include/queue.h include/utils.h

all: run

build: main.c ${FILES} ${HEADERS}
	gcc ${CFLAGS} -o tema2 main.c ${FILES} ${HEADERS} -lm

run: build
	./tema2 -c1 0 input.ppm output.dat

pack:
	zip -9 -FSr tema2.zip main.c ${FILES} ${HEADERS} Makefile README

clean:
	rm -rf tema2 tema2.zip
