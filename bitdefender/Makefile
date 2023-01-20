all: build

build: my_av

my_av: my_av.c ./src/tasks/task-one.c ./src/tasks/task-two.c ./src/utils/utils.c
	gcc -o my_av -g my_av.c ./src/tasks/task-one.c ./src/tasks/task-two.c ./src/utils/utils.c -lm -DVARIATION

my_av_var: my_av.c ./src/tasks/task-one.c ./src/tasks/task-two.c ./src/utils/utils.c
	gcc -o my_av my_av.c ./src/tasks/task-one.c ./src/tasks/task-two.c ./src/utils/utils.c -lm

run:
	./my_av

pack:
	zip -9 -FSr my_av.zip my_av.c ./src/tasks/task-one.c ./src/tasks/task-one.h ./src/tasks/task-two.c ./src/tasks/task-two.h ./src/utils/utils.c ./src/utils/utils.h ./src/utils/extensions.txt ./src/utils/tlds.txt ./src/utils/websites.txt my_av.py Makefile README

clean:
	rm -f my_av
