//
// Created by pvienne on 27/09/15.
//

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

int searchSolutions(int sudoku[SUDOKU_SIZE][SUDOKU_SIZE], int solutions[SUDOKU_SIZE][SUDOKU_SIZE]) {
    int i, j;
    int solutionsCount = 0;
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
            for (k = 0; k < SUDOKU_SIZE; k++) {
                if (seen[k] == 0) {
                    solutions[i][j] += 1 << k + 1;
                    solutionsCount++;
                }
            }
        }
    }
    return solutionsCount;
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

/**
 * Detects if x is a natural power of 2.
 * For explains about this code, see :
 * http://www.exploringbinary.com/ten-ways-to-check-if-an-integer-is-a-power-of-two-in-c/
 * Test n°9
 */
int isPowerOfTwo(int x) {
    return ((x != 0) && !(x & (x - 1)));
}

int own_log2(int x) {
    unsigned int ans = 0;
    while (x >>= 1) ans++;
    return ans;
}

void solveSudoku(int sudoku[SUDOKU_SIZE][SUDOKU_SIZE]) {
    solveSudokuRec(sudoku, NULL);
}

void solveSudokuRec(int sudoku[SUDOKU_SIZE][SUDOKU_SIZE], char *hasResult) {
    int cellToSolve = countCellToSolve(sudoku);
    while (cellToSolve > 0) {

        // Create the array which will contain solutions
        int solutions[SUDOKU_SIZE][SUDOKU_SIZE];
        int i;
        for (i = 0; i < SUDOKU_SIZE; i++) {
            initArray(solutions[i], SUDOKU_SIZE, 0);
        }

        // Search for solutions for this sudoku
        int count = searchSolutions(sudoku, solutions);

        // Put solutions in the sudoku
        int j;
        for (i = 0; i < SUDOKU_SIZE; i++) {
            for (j = 0; j < SUDOKU_SIZE; j++) {
                if (solutions[i][j] != 0 && isPowerOfTwo(solutions[i][j]))
                    sudoku[i][j] = own_log2(solutions[i][j]);
            }
        }

        // Check if we have solved something
        int cellToSolveBefore = cellToSolve;
        cellToSolve = countCellToSolve(sudoku);
        if (cellToSolve == cellToSolveBefore && count > 0) {
            // If sudoku is not simple, create branch on each solution
            // If one branch has successful solution, return it, unless set a dead flag !
            char goOut = 0;
            for (i = 0; i < SUDOKU_SIZE && !goOut; i++) {
                for (j = 0; j < SUDOKU_SIZE && !goOut; j++) {
                    if (solutions[i][j] != 0) {
                        int k;
                        for (k = 1; k < 10 && !goOut; k++) {
                            if (solutions[i][j] & (1 << k)) {
#if SUDOKU_DEBUG
                                printf("%d est une solution en %d,%d\n", k, i, j);
#endif
                                int sudokuChild[SUDOKU_SIZE][SUDOKU_SIZE];
                                copy_sudoku(sudoku, sudokuChild);
                                sudokuChild[i][j] = k;
                                char value = 0;
                                solveSudokuRec(sudokuChild, &value);
                                if (value == 1 && sudokuValide(sudokuChild)) {
                                    copy_sudoku(sudokuChild, sudoku);
                                    goOut = 1;
                                } else {
#if SUDOKU_DEBUG
                                    printf("Impossible de le faire !\n");
#endif
                                }
                            }
                        }
                    }
                }
            }

#if SUDOKU_DEBUG
            printf("Sudoku is not simple !\n");
#endif
            return;
        } else if (count == 0) {
            return;
        }
    }
    if (hasResult != NULL && sudokuValide(sudoku))
        *hasResult = 1;
}

void copy_sudoku(int parent[SUDOKU_SIZE][SUDOKU_SIZE], int child[SUDOKU_SIZE][SUDOKU_SIZE]) {
    int i, j;
    for (i = 0; i < SUDOKU_SIZE; i++) {
        for (j = 0; j < SUDOKU_SIZE; j++) {
            child[i][j] = parent[i][j];
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
