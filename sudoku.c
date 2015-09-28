//
// Created by pvienne on 27/09/15.
//

#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"

#define SUDOKU_ERROR 255

int sudokuValide(int sudoku[SUDOKU_SIZE][SUDOKU_SIZE]) {

    int i, j, rCol, rLin;
    /*
     * Each seen array define if they have seen, on (line, row, region) index+1, the number index+1
     */
    int seen[SUDOKU_SIZE];

    initArray(seen, SUDOKU_SIZE, 0);

    // Check all lines
    for (i = 0; i < SUDOKU_SIZE; i++) {
        for (j = 0; j < SUDOKU_SIZE; j++) {
            if (sudoku[i][j] > 0) {
                seen[sudoku[i][j] - 1]++;
                if (seen[sudoku[i][j] - 1] > 1) {
#if SUDOKU_DEBUG
                    printf("Invalid on line on (%d,%d)\n", i, j);
#endif
                    return 0;
                }
            }
        }
        initArray(seen, SUDOKU_SIZE, 0);
    }

    // Check all columns
    for (i = 0; i < SUDOKU_SIZE; i++) {
        for (j = 0; j < SUDOKU_SIZE; j++) {
            if (sudoku[j][i] > 0) {
                seen[sudoku[j][i] - 1]++;
                if (seen[sudoku[j][i] - 1] > 1) {
#if SUDOKU_DEBUG
                    printf("Invalid on column on (%d,%d)\n", j, i);
#endif
                    return 0;
                }
            }
        }
        initArray(seen, SUDOKU_SIZE, 0);
    }

    // Check all regions
    /* rCol and rLin are the start position for each region (on the top left corner)
     * i and j goes from 0 to 2 to navigate on all cells inside a region
     */
    for (rCol = 0; rCol < SUDOKU_SIZE; rCol += 3) {
        for (rLin = 0; rLin < SUDOKU_SIZE; rLin += 3) {
            for (i = 0; i < (SUDOKU_SIZE / 3); i++) {
                for (j = 0; j < (SUDOKU_SIZE / 3); j++) {
                    if (sudoku[rLin + i][rCol + j] > 0) {
                        seen[sudoku[rLin + i][rCol + j] - 1]++;
                        if (seen[sudoku[rLin + i][rCol + j] - 1] > 1) {
#if SUDOKU_DEBUG
                            printf("Invalid on region on (%d,%d)\n", rLin + i, rCol + j);
#endif
                            return 0;
                        }
                    }
                }
            }
            initArray(seen, SUDOKU_SIZE, 0);
        }
    }

    return 1;
}

void searchSolutions(int sudoku[SUDOKU_SIZE][SUDOKU_SIZE], int solutions[SUDOKU_SIZE][SUDOKU_SIZE]) {
    int i, j;
    for (i = 0; i < SUDOKU_SIZE; i++) {
        for (j = 0; j < SUDOKU_SIZE; j++) {
            if (sudoku[i][j] > 0)
                continue;
            int seen[SUDOKU_SIZE];
            initArray(seen, SUDOKU_SIZE, 0);
            elementsInColumn(seen, sudoku, j);
            elementsInLine(seen, sudoku, i);
            elementsInRegion(seen, sudoku, i - (i % (SUDOKU_SIZE / 3)), j - (j % (SUDOKU_SIZE / 3)));
            int k = 0;
            int hasSolution = 0;
            for (k = 0; k < SUDOKU_SIZE; k++) {
                if (seen[k] == 0) {
                    if (hasSolution == 0) {
                        solutions[i][j] = k + 1;
                        hasSolution = 1;
                    } else if (hasSolution == 1) {
                        solutions[i][j] = 0;
                        hasSolution = -1;
                    }
                }
            }
        }
    }
}

int countCellToSolve(int sudoku[SUDOKU_SIZE][SUDOKU_SIZE]) {
    int i, j, c = 0;
    for (i = 0; i < SUDOKU_SIZE; i++) {
        for (j = 0; j < SUDOKU_SIZE; j++) {
            if (sudoku[i][j] == 0) {
                c++;
            }
        }
    }
    return c;
}

void solveSudoku(int sudoku[SUDOKU_SIZE][SUDOKU_SIZE]) {
    int cellToSolve = countCellToSolve(sudoku);
    while (cellToSolve > 0) {

        // Create the array which will contain solutions
        int solutions[SUDOKU_SIZE][SUDOKU_SIZE];
        int i;
        for (i = 0; i < SUDOKU_SIZE; i++) {
            initArray(solutions[i], SUDOKU_SIZE, 0);
        }

        // Search for solutions for this sudoku
        searchSolutions(sudoku, solutions);

        // Put solutions in the sudoku
        int j;
        for (i = 0; i < SUDOKU_SIZE; i++) {
            for (j = 0; j < SUDOKU_SIZE; j++) {
                if (solutions[i][j] != 0)
                    sudoku[i][j] = solutions[i][j];
            }
        }

        // Check if we have solved something
        int cellToSolveBefore = cellToSolve;
        cellToSolve = countCellToSolve(sudoku);
        if (cellToSolve == cellToSolveBefore) { // If sudoku is not simple, stop computing
#if SUDOKU_DEBUG
            fprintf(stderr, "The sudoku is not simple!");
#endif
            break;
        }
    }
}

void elementsInLine(int seen[SUDOKU_SIZE], int sudoku[SUDOKU_SIZE][SUDOKU_SIZE], int line) {
    int i;
    for (i = 0; i < SUDOKU_SIZE; i++) {
        if (sudoku[line][i] > 0 && sudoku[line][i] <= SUDOKU_SIZE) {
            seen[sudoku[line][i] - 1]++;
        }
    }
}

void elementsInColumn(int seen[SUDOKU_SIZE], int sudoku[SUDOKU_SIZE][SUDOKU_SIZE], int column) {
    int i;
    for (i = 0; i < SUDOKU_SIZE; i++) {
        if (sudoku[i][column] > 0 && sudoku[i][column] <= SUDOKU_SIZE) {
            seen[sudoku[i][column] - 1]++;
        }
    }
}


void elementsInRegion(int seen[SUDOKU_SIZE], int sudoku[SUDOKU_SIZE][SUDOKU_SIZE], int regionStartLine,
                      int regionStartColumn) {

    // Check arguments
    if (regionStartColumn % (SUDOKU_SIZE / 3) != 0 || regionStartLine % (SUDOKU_SIZE / 3) != 0) {
#if SUDOKU_DEBUG
        fprintf(stderr, "Error on region start, asked for (%d,%d)", regionStartLine + 1, regionStartColumn + 1);
#endif
        exit(SUDOKU_ERROR);
    }

    int i, j;
    for (i = 0; i < SUDOKU_SIZE / 3; i++) {
        for (j = 0; j < SUDOKU_SIZE / 3; j++) {
            int value = sudoku[regionStartLine + i][regionStartColumn + j];
            if (value > 0 && value <= SUDOKU_SIZE) {
                seen[value - 1]++;
            }
        }
    }
}

void initArray(int array[], int size, int value) {
    int i;
    for (i = 0; i < size; i++) {
        array[i] = value;
    }
}