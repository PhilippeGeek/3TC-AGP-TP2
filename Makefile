CFLAGS = -g -Wall -DDEBUG
LDLAGS = -g -Wall -DDEBUG

all: lireSudoku #resoudreSudoku 

utilSudoku.o: utilSudoku.c utilSudoku.h
	gcc ${CFLAGS} -c utilSudoku.c  -o utilSudoku.o

main.o:	main.c
	gcc ${CFLAGS} -c main.c -o main.o

lireSudoku: main.o utilSudoku.o
	gcc ${LDFLAGS} main.o utilSudoku.o -o lireSudoku

resoudreSudoku: sudoku.o utilSudoku.o
	gcc ${LDFLAGS} sudoku.o   utilSudoku.o -o resoudreSudoku

sudoku.o: sudoku.c sudoku.h
	gcc ${CFLAGS} -c sudoku.c  -o sudoku.o

tar-sol: 
	cd ..; tar cvf  TP2-code.tar prog/main.c prog/utilSudoku.c prog/utilSudoku.h prog/Makefile prog/sudokuInit.txt  prog/sudoku1.txt prog/sudoku.c

tar: 
	cd ..; tar cvf  TP2.tar prog/main.c prog/utilSudoku.c prog/utilSudoku.h prog/Makefile prog/sudokuInit.txt  prog/sudoku1.txt 

clean: 
	\rm -rf *.o sudoku solution *~
