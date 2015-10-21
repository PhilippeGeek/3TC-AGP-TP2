//
// Created by pvienne on 27/09/15.
//

#ifndef TP2_SUDOKU_H
#define TP2_SUDOKU_H
#define SUDOKU_SIZE 9
#define SUDOKU_DEBUG 0

void solveSudokuRec(int sudoku[SUDOKU_SIZE][SUDOKU_SIZE], char *hasResult);

void copy_sudoku(int pInt[SUDOKU_SIZE][SUDOKU_SIZE], int child[SUDOKU_SIZE][SUDOKU_SIZE]);

int sudokuValide(int sudoku[SUDOKU_SIZE][SUDOKU_SIZE]);

int searchSolutions(int sudoku[SUDOKU_SIZE][SUDOKU_SIZE], int solutions[SUDOKU_SIZE][SUDOKU_SIZE]);

void solveSudoku(int sudoku[SUDOKU_SIZE][SUDOKU_SIZE]);

void elementsInLine(int seen[SUDOKU_SIZE], int sudoku[SUDOKU_SIZE][SUDOKU_SIZE], int line);

void elementsInRegion(int seen[SUDOKU_SIZE], int sudoku[SUDOKU_SIZE][SUDOKU_SIZE], int regionStartLine,
                      int regionStartColumn);

void elementsInColumn(int seen[SUDOKU_SIZE], int sudoku[SUDOKU_SIZE][SUDOKU_SIZE], int column);

void initArray(int array[], int size, int value);

#endif //TP2_SUDOKU_H
