//   Author: Sean Comben, Joshua Martinez
//   School: UIC
//   Class : CS211
//   Term  : Fall 2021
//   Professor : John Lillis & Patrick Troy
//
//   Balloon popping functions

// file:  bpgame.c


/** #include statements... **/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bpgame.h"
#define MAX_ROWS 40
#define MAX_COLS 40
#define None '.'    // "air"
#define Red  '^'
#define Blue '='
#define Green 'o'
#define Yellow '+'

/** TYPEDEFS / STRUCTs HERE ***/
typedef struct bpgame BPGame;

struct bpgame {
    // YOU DECIDE WHAT TO PUT IN HERE TO CAPTURE THE STATE OF
    //   A "RUN" OF THE GAME.....
    //create a 2d array type char
    //nrows, ncols
    char** array2d;
    int nrows, ncols, score, a;

};

// Function required for auto-grader
void bp_display_STD(BPGame* b) {
    for (int i = 0; i < b->nrows; i++) {
        for (int j = 0; j < b->ncols; j++) {
            printf("%c", b->array2d[i][j]);
        }
        printf("\n");
    }
}

/*** IMPLEMENTATION OF bp_XXXX FUNCTIONS HERE  ****/

//create random balloons to be inserted in create
void randballons(int nrows, int ncols, BPGame* b) {
    int totalsquares = nrows * ncols;
    char a;
    int c = 0;

    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            c = rand() % 4;
            if (c == 0) {
                a = Red;
            }
            else if (c == 1) {
                a = Blue;
            }
            else if (c == 2) {
                a = Green;
            }
            else if (c == 3) {
                a = Yellow;
            }
            b->array2d[i][j] = a;
        }
    }

}

// create a grid of size nrows and ncols with randomly inserted balloons
extern BPGame* bp_create(int nrows, int ncols) {

    if (nrows > MAX_ROWS || ncols > MAX_COLS) {
        printf("Error, number inputted too high");
        return NULL;
    }

    // initialize BPGame variable
    BPGame* b;
    b = (BPGame*)malloc(sizeof(BPGame));
    b->nrows = nrows;
    b->ncols = ncols;
    b->score = 0;
    b->a = 0; //balloon popped counter
    b->array2d = (char**)malloc(sizeof(char*) * nrows);
    for (int i = 0; i < nrows; i++) {
        b->array2d[i] = (char*)malloc(sizeof(char) * ncols);
    }
    randballons(nrows, ncols, b);

    return b;
}

/*
* Function:  bp_create_from_mtx
* Parameters: number of rows and columns and 2D array of char
*/
extern BPGame* bp_create_from_mtx(char mtx[][MAX_COLS], int nrows, int ncols) {
    // check if nrows or ncols is out of bounds
    if (ncols > MAX_COLS) {
        printf("ERROR: Variable 'ncols' is out of bound.");
        return NULL;
    }

    // initialize BPGame variable to later be returned
    BPGame* ptr_board;
    ptr_board = (BPGame*)malloc(sizeof(BPGame));
    ptr_board->nrows = nrows;
    ptr_board->ncols = ncols;
    ptr_board->array2d = (char**)malloc(sizeof(char*) * nrows);
    for (int i = 0; i < nrows; i++) {
        ptr_board->array2d[i] = (char*)malloc(sizeof(char) * ncols);
    }

    // copy data from argument matrix onto 2d array inside our BPGame pointer
    for (int i = 0; i < nrows; i++) {

        for (int j = 0; j < ncols; j++)
        {
            if (mtx[i][j] != Red && mtx[i][j] != Blue && mtx[i][j] != Green && mtx[i][j] != Yellow && mtx[i][j] != None)
            {
                printf("ERROR: Element within array is unknown.");
                return NULL;
            }
            else {
                ptr_board->array2d[i][j] = mtx[i][j];
            }
        }

    }
    return ptr_board;
}

/**
* Function:  bp_destroy
* Description:  deallocates all dynamically allocated
*  		memory associated with the board.
*
*		Comment:  this might start out as
*		a one-liner.
*
*/
extern void bp_destroy(BPGame* b) {

    for (int i = 0; i < b->nrows; i++) {
        free(b->array2d[i]);
        //  printf("I value: %d\n", i);
    }
    b->ncols = 0;
    b->nrows = 0;
    free(b->array2d);
    b->array2d = NULL;
}

/*
* Function:  bp_display
* Description:  pretty-prints the current board.
*/
extern void bp_display(BPGame* b) {
    int numRows = b->nrows;
    int numCols = b->ncols;

    // First line : +----+
    printf("  +");
    for (int i = 0; i < (numCols * 2) + 1; i++) {
        printf("-");
    }
    printf("+\n");

    // Content of board: ^ + + o
    for (int i = 0; i < numRows; i++) {
        printf("%d | ", i); // "0 | "
        for (int j = 0; j < numCols; j++) {
            printf("%c ", b->array2d[i][j]);
        }
        printf("|\n"); // "|"
    }

    // Last line: +----+
    printf("  +");
    for (int i = 0; i < (numCols * 2) + 1; i++) {
        printf("-");
    }
    printf("+\n");

    // 1st horizontal line
    printf("    ");
    for (int i = 0; i < numCols - 1; i++) {
        printf("0 ");
    }
    printf("1\n");

    // 2nd horizontal line
    printf("    ");
    int counter = 0;
    for (int i = 0; i < numCols; i++) {
        if (counter > 9) counter = 0;
        printf("%d ", counter);
        counter++;
    }
    printf("\n");
}

// recursive calling to change the balloons popped into None
void _recursive(BPGame* b, int r, int c, char storing) {

    b->array2d[r][c] = None;  // set to air
    //checks if surrounding ballon is the same as the one picked
    if (r > 0 && b->array2d[r - 1][c] == storing) {
        b->a++; // keeping count of score
        _recursive(b, r - 1, c, storing);
    }
    if (r < b->nrows - 1 && b->array2d[r + 1][c] == storing) {
        b->a++;
        _recursive(b, r + 1, c, storing);
    }
    if (c > 0 && b->array2d[r][c - 1] == storing) {
        b->a++;
        _recursive(b, r, c - 1, storing);
    }
    if (c < b->ncols - 1 && b->array2d[r][c + 1] == storing) {
        b->a++;
        _recursive(b, r, c + 1, storing);
    }

    if (b->a == 0) {
        b->array2d[r][c] = storing;
    }
}

// pop function
extern int bp_pop(BPGame* b, int r, int c) {
    //checks if the place being popped is equal to None
    if (b->array2d[r][c] == None) {
        b->a = 0;
        return b->a;
    }
    char storing = b->array2d[r][c];
    b->a = 0;

    _recursive(b, r, c, storing);
    
    //updates score
    b->score += (b->a + 1) * ((b->a + 1) - 1);

    return b->a + 1;
}


/*
* Function:  bp_is_compact
* Description:  determines if all balloons are as "high" as possible.
*/
extern int bp_is_compact(BPGame* b) {
    int true = 1, false = 0, numRows = b->nrows, numCols = b->ncols;

    for (int i = 1; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            // temp = one "above" the current element we are inspecting
            char curr = b->array2d[i][j];
            char temp = b->array2d[i - 1][j];
            if (curr != None && temp == None) {
                return false;
            }
        }
    }
    return true;
}

//bumps balloons up if there's nothing above the balloons
extern void bp_float_one_step(BPGame* b) {

    //goes through each balloon checking if the top of it is empty.
    for (int i = 0; i < b->nrows; i++) {
        for (int j = 0; j < b->ncols; j++) {

            if (i == 0) {
                if (b->array2d[i][j] == None) {
                    b->array2d[i][j] = b->array2d[i + 1][j];
                    b->array2d[i + 1][j] = None;
                }
            }
            else if (i > 0) {
                if (b->array2d[i - 1][j] == None) {
                    b->array2d[i - 1][j] = b->array2d[i][j];
                    b->array2d[i][j] = None;
                }
            }


        }
    }
}


/*
* Function:  bp_score
* Description:  returns the current score
*/
extern int bp_score(BPGame* b) {
    return b->score;
}

// gets the balloon at the location that was inputted
extern int bp_get_balloon(BPGame* b, int r, int c) {
    if (b->array2d[r][c] == '^') {
        return '^';
    }
    else if (b->array2d[r][c] == '=') {
        return '=';
    }
    else if (b->array2d[r][c] == 'o') {
        return 'o';
    }
    else if (b->array2d[r][c] == '+') {
        return '+';
    }
    else {
        return -1;
    }
}


/*
* Helper Function: has_neighbors
* Description: returns true or false, depending on if element of array has a neighbor
*/
int has_neighbors(BPGame* b, int row_location, int col_location)
{
    int true = 1, false = 0;

    char curr = b->array2d[row_location][col_location];
    if (curr == None) return false;

    if (col_location == 0) {
        if (row_location == 0) {
            if (curr == b->array2d[row_location][col_location + 1]) return true;
            if (curr == b->array2d[row_location + 1][col_location]) return true;
        }
        else if (row_location == b->nrows - 1) {
            if (curr == b->array2d[row_location][col_location + 1]) return true;
            if (curr == b->array2d[row_location - 1][col_location]) return true;
        }
        else {
            if (curr == b->array2d[row_location - 1][col_location]) return true;
            if (curr == b->array2d[row_location][col_location + 1]) return true;
            if (curr == b->array2d[row_location + 1][col_location]) return true;
        }
    }
    else if (col_location == b->ncols) {
        if (row_location == 0) {
            if (curr == b->array2d[row_location][col_location - 1]) return true;
            if (curr == b->array2d[row_location + 1][col_location]) return true;
        }
        else if (row_location == b->nrows) {
            if (curr == b->array2d[row_location][col_location - 1]) return true;
            if (curr == b->array2d[row_location - 1][col_location]) return true;
        }
        else {
            if (curr == b->array2d[row_location - 1][col_location]) return true;
            if (curr == b->array2d[row_location][col_location - 1]) return true;
            if (curr == b->array2d[row_location + 1][col_location]) return true;
        }
    }
    else if (row_location == 0) {
        if (col_location == 0) {
            if (curr == b->array2d[row_location][col_location + 1]) return true;
            if (curr == b->array2d[row_location + 1][col_location]) return true;
        }
        else if (row_location == b->nrows - 1) {
            if (curr == b->array2d[row_location][col_location - 1]) return true;
            if (curr == b->array2d[row_location + 1][col_location]) return true;
        }
        else {
            if (curr == b->array2d[row_location + 1][col_location]) return true;
            if (curr == b->array2d[row_location][col_location - 1]) return true;
            if (curr == b->array2d[row_location][col_location + 1]) return true;
        }
    }
    else if (row_location == b->nrows) {
        if (curr == b->array2d[row_location - 1][col_location]) return true;
        if (curr == b->array2d[row_location][col_location - 1]) return true;
        if (curr == b->array2d[row_location][col_location + 1]) return true;
    }
    else {
        if (curr == b->array2d[row_location - 1][col_location]) return true;
        if (curr == b->array2d[row_location + 1][col_location]) return true;
        if (curr == b->array2d[row_location][col_location - 1]) return true;
        if (curr == b->array2d[row_location][col_location + 1]) return true;
    }

    return false;
}

/**
* Function:   bp_can_pop
* Description:  returns 1 if the player can pop some balloons, 0 otherwise.
*/
extern int bp_can_pop(BPGame* b) {
    int true = 1;
    int false = 0;

    // check for empty aray/ None array
    int is_empty = true;
    for (int i = 0; i < b->nrows; i++) {
        for (int j = 0; j < b->ncols; j++) {
            if (b->array2d[i][j] != None) is_empty = false;
        }
    }
    if (is_empty == true) return false;

    // check every element if it has a neighbor
    int element_has_neighors = false;
    for (int i = 0; i < b->nrows; i++) {
        for (int j = 0; j < b->ncols; j++) {
            element_has_neighors = has_neighbors(b, i, j);
            if (element_has_neighors == true) return true;
        }
    }

    return false;
}

extern int bp_undo(BPGame* b) {
    return 0;
}