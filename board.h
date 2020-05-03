#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include "types.h"
#include "errors.h"
#define BOARD_SIZE 9
#define MAX_LENGTH 20
#define NUMBER_OF_LINES 19
#define LENGTH_OF_LINE 38
#define BOARD_EDGE "U===========U===========U===========U\n"
#define BETWEEN_ROWS "U---+---+---U---+---+---U---+---+---U\n"

typedef struct {
    size_t board[BOARD_SIZE][BOARD_SIZE];
    size_t initial_board[BOARD_SIZE][BOARD_SIZE];
    bool rules;
}board_t;

//Initialize the sudoku
status_t board_init(board_t* game);

//Reset and get back to initial board
status_t board_reset(board_t* game);

//Responsible for checking the rule set
status_t board_check_rules(board_t* game, 
                           bool* rules, 
                           size_t numb, 
                           size_t row, 
                           size_t col);

//Checks for repeated numbers in the row
status_t board_check_rows(board_t* game, 
                          bool* rules,
                          size_t numb, 
                          size_t row);

//Checks for repeated numbers in the column
status_t board_check_columns(board_t* game,
                             bool* rules,
                             size_t numb, 
                             size_t col);

//Checks for repeated numbers in the same position in other cells
status_t board_check_positions(board_t* game, 
                               bool* rules,
                               size_t numb, 
                               size_t row, 
                               size_t col);

//Check for repeted numbers on the grill
status_t board_check_grill(board_t* game, 
                           bool* rules,
                           size_t numb, 
                           size_t row, 
                           size_t col);

//If the position is free,then it puts the value in the row and column required
status_t board_put_value(board_t* game, size_t numb, size_t row, size_t col);

//Load the board in a buffer to send to the client
status_t board_serialize(board_t* game, 
                         uint8_t* buff);

//Load one specific row in the buffer that would be send to the client
status_t board_generate_row(board_t* game,
                            uint8_t* buff, 
                            size_t row);

#endif
