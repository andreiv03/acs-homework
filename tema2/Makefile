all: tema2

tema2: tema2.o intern.o task1.o task2.o task3.o task4.o task5.o
	gcc tema2.o intern.o task1.o task2.o task3.o task4.o task5.o -o tema2 -lm -Wall

tema2.o: tema2.c
	gcc -c tema2.c

intern.o: intern.c
	gcc -c intern.c

task1.o: task1.c
	gcc -c task1.c

task2.o: task2.c
	gcc -c task2.c

task3.o: task3.c
	gcc -c task3.c

task4.o: task4.c
	gcc -c task4.c

task5.o: task5.c
	gcc -c task5.c

clean:
	rm *.o tema2


zip:
	zip avioane.zip task1.c task2.c task3.c \
	task4.c task5.c utils.h README Makefile
