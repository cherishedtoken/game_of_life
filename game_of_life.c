#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

// Total number of rows and columns.
#define GAMEBOARD_SIZE 20
// Return value when a cell lies outside the grid.
#define OUT_OF_BOUNDS 0
// If a cell remains the same for STABLE_CYCLES cycles, flip its value.
#define STABLE_CYCLES 20

int getNorthNeighbor(int** gameboard, int x, int y) {
    if (y == 0) {
        return OUT_OF_BOUNDS;
    } else {
        return gameboard[x][y - 1];
    }
}

int getSouthNeighbor(int** gameboard, int x, int y) {
    if (y == GAMEBOARD_SIZE - 1) {
        return OUT_OF_BOUNDS;
    } else {
        return gameboard[x][y + 1];
    }
}

int getWestNeighbor(int** gameboard, int x, int y) {
    if (x == 0) {
        return OUT_OF_BOUNDS;
    } else {
        return gameboard[x - 1][y];
    }
}

int getEastNeighbor(int** gameboard, int x, int y) {
    if (x == GAMEBOARD_SIZE - 1) {
        return OUT_OF_BOUNDS;
    } else {
        return gameboard[x + 1][y];
    }
}

int getNeighbors(int** gameboard, int x, int y) {
    return getNorthNeighbor(gameboard, x, y) + getSouthNeighbor(gameboard, x, y) +
        getWestNeighbor(gameboard, x, y) + getEastNeighbor(gameboard, x, y);
}

void print_gameboard(int** gameboard) {
    for (int i = 0; i < GAMEBOARD_SIZE; i++) {
        for (int j = 0; j < GAMEBOARD_SIZE; j++) {
            printf("%d ", gameboard[i][j]);
        }
        printf("\n");
    }

    printf("\n"); // Separate the different instances of the board
}

int** create_gameboard() {
    int** gameboard;

    gameboard = malloc(GAMEBOARD_SIZE * sizeof(int*));
    if (!gameboard) abort();

    for (int i = 0; i < GAMEBOARD_SIZE; i++) {
        gameboard[i] = malloc(GAMEBOARD_SIZE * sizeof(int));
        if (!gameboard[i]) abort();
    }

    return gameboard;
}

void initialize_gameboard(int** gameboard) {
    // Now randomly decide which elements should be given life.
    srand(time(NULL));

    for (int i = 0; i < GAMEBOARD_SIZE; i++) {
        for (int j = 0; j < GAMEBOARD_SIZE; j++) {
            gameboard[i][j] = rand() % 2;
        }
    }
}

void free_array(int** gameboard) {
    for (int i = 0; i < GAMEBOARD_SIZE; i++) {
        free(gameboard[i]);
    }
    free(gameboard);
}

int main() {
    int** gameboard = create_gameboard();
    initialize_gameboard(gameboard);

    int** gameboard_history = create_gameboard();
    for (int i = 0; i < GAMEBOARD_SIZE; i++) {
        for (int j = 0; j < GAMEBOARD_SIZE; j++) {
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
        for (int i = 0; i < GAMEBOARD_SIZE; i++) {
            for (int j = 0; j < GAMEBOARD_SIZE; j++) {
                gameboard_copy[i][j] = gameboard[i][j];
            }
        }


        for (int i = 0; i < GAMEBOARD_SIZE; i++) {
            for (int j = 0; j < GAMEBOARD_SIZE; j++) {
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
        for (int i = 0; i < GAMEBOARD_SIZE; i++) {
            for (int j = 0; j < GAMEBOARD_SIZE; j++) {
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
