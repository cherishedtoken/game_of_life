#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

// Total number of rows and columns.
#define ROWS 30
#define COLUMNS 60
// Return value when a cell lies outside the grid.
#define OUT_OF_BOUNDS 0
// If a cell remains the same for STABLE_CYCLES cycles, flip its value.
#define STABLE_CYCLES 20

int getNorthNeighbor(int** gameboard, int row, int column) {
    if (row == 0) {
        return OUT_OF_BOUNDS;
    } else {
        return gameboard[row - 1][column];
    }
}

int getSouthNeighbor(int** gameboard, int row, int column) {
    if (row == ROWS - 1) {
        return OUT_OF_BOUNDS;
    } else {
        return gameboard[row + 1][column];
    }
}

int getWestNeighbor(int** gameboard, int row, int column) {
    if (column == 0) {
        return OUT_OF_BOUNDS;
    } else {
        return gameboard[row][column - 1];
    }
}

int getEastNeighbor(int** gameboard, int row, int column) {
    if (column == COLUMNS - 1) {
        return OUT_OF_BOUNDS;
    } else {
        return gameboard[row][column + 1];
    }
}

int getNeighbors(int** gameboard, int row, int column) {
    return getNorthNeighbor(gameboard, row, column) +
        getSouthNeighbor(gameboard, row, column) +
        getWestNeighbor(gameboard, row, column) +
        getEastNeighbor(gameboard, row, column);
}

void print_gameboard(int** gameboard) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            printf("%d ", gameboard[i][j]);
        }
        printf("\n");
    }

    printf("\n"); // Separate the different instances of the board
}

int** create_gameboard() {
    int** gameboard;

    gameboard = malloc(ROWS * sizeof(int*));
    if (!gameboard) abort();

    for (int i = 0; i < ROWS; i++) {
        gameboard[i] = malloc(COLUMNS * sizeof(int));
        if (!gameboard[i]) abort();
    }

    return gameboard;
}

void initialize_gameboard(int** gameboard) {
    // Now randomly decide which elements should be given life.
    srand(time(NULL));

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            gameboard[i][j] = rand() % 2;
        }
    }
}

void free_array(int** gameboard) {
    for (int i = 0; i < ROWS; i++) {
        free(gameboard[i]);
    }
    free(gameboard);
}

int main() {
    int** gameboard = create_gameboard();
    initialize_gameboard(gameboard);

    int** gameboard_history = create_gameboard();
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            gameboard_history[i][j] = 0;
        }
    }

    /* Here are the rules of the game:
       1. Any live cell with fewer than two live neighbors dies, as if caused by
       under-population.
       2. Any live cell with two or three live neighbors lives on to the next
       generation.
       3. Any live cell with more than three live neighbors dies, as if by
       over-population.
       4. Any dead cell with exactly three live neighbors becomes a live cell,
       as if by reproduction.
    */

    while (1) {
        int** gameboard_copy = create_gameboard();
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                gameboard_copy[i][j] = gameboard[i][j];
            }
        }


        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                int neighbors = getNeighbors(gameboard_copy, i, j);
                if (!gameboard_copy[i][j] && neighbors == 3) {
                    gameboard[i][j] = 1;
                } else if (gameboard_copy[i][j]) {
                    if (neighbors < 2 || neighbors > 3) {
                        gameboard[i][j] = 0;
                    } else if (neighbors == 2 || neighbors == 3) {
                        continue;
                    }
                }
            }
        }

        // If a particular cell remains the same for n turns, flip its value.
        // This is done to make things more... interesting.
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS; j++) {
                if (gameboard_history[i][j] >= STABLE_CYCLES) {
                    gameboard[i][j] = !gameboard[i][j];
                    gameboard_history[i][j] = 0;
                } else {
                    gameboard_history[i][j]++;
                }
            }
        }

        free_array(gameboard_copy);
        print_gameboard(gameboard);
        sleep(1); // Give us enough time to see the update.
    }
}
