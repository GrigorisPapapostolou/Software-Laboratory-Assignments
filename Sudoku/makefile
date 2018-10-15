CC=gcc
CFLAGS=-I.
CCFLAGS= -Wall -ansi -pedantic -g -c -o

split.o: split.c shell.h split.h
	gcc -c split.c 

run.o: run.c shell.h run.h
	gcc -c run.c

main.o: main.c shell.h run.h split.h
	gcc -c main.c

exe: main.o grid.o sudoku.o 
	gcc –Wall –ansi –pedantic –o exe split.o run.o main.o

clean:
	rm *.o exe
