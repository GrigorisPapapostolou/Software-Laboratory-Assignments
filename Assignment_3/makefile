# Makefile
# Symbol Table ADT
# @ralexstokes

# Macros
CC = gcc
CCFLAGS = -g

all: testsymtablelist testsymtablehash

clean:
	rm testsymtablehash testsymtablelist *.o

testsymtablelist: testsymtable.o symtablelist.o
	$(CC) $(CCFLAGS) $< symtablelist.o -o $@

testsymtablehash: testsymtable.o symtablehash.o
	$(CC) $(CCFLAGS) $< symtablehash.o -o $@

testsymtable.o: symtable.h
symtablelist.o: symtable.h
symtablehash.o: symtable.h
