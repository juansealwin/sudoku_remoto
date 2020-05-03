#include "board.h"

status_t board_init(board_t* game){
    if (game == NULL)
        return ERROR_NULL_POINTER;

    FILE* board_txt;
    char line[MAX_LENGTH];
    size_t i = 0;
    size_t row = 0;
    size_t column;

    if ((board_txt = fopen("board.txt", "r")) == NULL) {
        return ERROR_FILE_OPENING;
    }

    while ((fgets(line, MAX_LENGTH, board_txt)) != NULL) {
        for (i = 0 ; line[i] != '\n' && line[i] != '\0' ; i++) {
            if (line[i] == ' ') {
                continue;
            } else {
                game->board[row][column] = atoi(&line[i]);
                game->initial_board[row][column] = game->board[row][column];
                column++;
            }
        }
        row ++;
        column = 0;
    }
    game->rules = true;
    fclose(board_txt);
    return OK;
}

status_t board_reset(board_t* game){
    if (game == NULL)
        return ERROR_NULL_POINTER;

    for (size_t i = 0 ; i < BOARD_SIZE ; i++) {
        for (size_t j = 0; j < BOARD_SIZE; j++) {
            game->board[i][j] = game->initial_board[i][j];
        }
    }
    game->rules = true;
    return OK;
}

status_t board_put_value(board_t* game, size_t numb, size_t row, size_t col ){
    if (game == NULL)
        return ERROR_NULL_POINTER;

    if ((game->board[row-1][col-1]) == 0) 
        game->board[row-1][col-1] = numb;
    else 
        return ERROR_FULL_TILE;
    
    return OK;
}

status_t board_check_rules(board_t* game, 
                           bool* rules, 
                           size_t numb, 
                           size_t row, 
                           size_t col){
    if (game == NULL || rules == NULL)
        return ERROR_NULL_POINTER;

    status_t st;

    if ((st = (board_check_rows(game, rules, numb, row))) != OK) {
        return st;
    }
    if ((st = (board_check_columns(game, rules, numb, col))) != OK) {
        return st;
    }
    /*if ((st = (board_check_positions(game, 
                                     rules, 
                                     numb, 
                                     row, 
                                     col))) != OK) {
        return st;
    }*/
    if ((st = (board_check_grill(game, 
                                 rules, 
                                 numb, 
                                 row, 
                                 col))) != OK) {
        return st;
    }
    return OK;
}

status_t board_check_rows(board_t* game, 
                          bool* rules,
                          size_t numb, 
                          size_t row){
    if (game == NULL || rules == NULL)
        return ERROR_NULL_POINTER;
    size_t cant = 0; 

    for (size_t i = 0 ; i < BOARD_SIZE ; i++) {
        if (game->board[row-1][i] == numb)
            cant++;
    }
    if (cant > 1)
        *rules = false;

    return OK;    
}

status_t board_check_columns(board_t* game,
                             bool* rules,
                             size_t numb, 
                             size_t col){
    if (game == NULL || rules == NULL)
        return ERROR_NULL_POINTER;                             
    size_t cant = 0; 
    
    for (size_t i = 0 ; i < BOARD_SIZE ; i++) {
        if (game->board[i][col-1] == numb)
            cant++;
    }
    if (cant > 1)
        *rules = false; 
    return OK;
}

status_t board_check_positions(board_t* game, 
                               bool* rules,
                               size_t numb, 
                               size_t row, 
                               size_t col){
    if (game == NULL || rules == NULL)
        return ERROR_NULL_POINTER;                               
    size_t i, j;
    size_t cant = 0;

    for (i = (row-1) % 3; i < BOARD_SIZE ; i += 3) {
        for (j = (col-1) % 3 ; j < BOARD_SIZE ; j += 3) {
            if (game->board[i][j] == numb)
                cant++;
        }
    }
    if (cant > 1)
        *rules = false;
    return OK;
}

status_t board_check_grill(board_t* game, 
                           bool* rules,
                           size_t numb, 
                           size_t row, 
                           size_t col){
    if (game == NULL || rules == NULL)
        return ERROR_NULL_POINTER;     

    size_t cant = 0;

    if ((row % 3) == 0)
        row -= 3;
    else if ((row % 3) == 2)
        row -= 2;

    if ((col % 3) == 0)
        col -= 3;
    else if ((col % 3) == 2)
        col -= 2;

    for (size_t i = 0; i < 3 ; i ++) {
        for (size_t j = 0 ; j < 3 ; j ++) {
            if (game->board[row+i][col+j] == numb)
                cant++;
        }
    }
    if (cant > 1)
        *rules = false;
    return OK;
}

status_t board_serialize(board_t* game, uint8_t* buff){
    if (game == NULL || buff == NULL)
        return ERROR_NULL_POINTER;

    status_t st;
    size_t row = 0;
    size_t i;

    for (i = 0 ; i < NUMBER_OF_LINES-1 ; i++) {
        strncpy((char*)&buff[i*LENGTH_OF_LINE],
                            BOARD_EDGE,
                            LENGTH_OF_LINE);
        i++;
        if ((st =(board_generate_row(game,
                                     &buff[i*LENGTH_OF_LINE],
                                     row))) != OK)
            return st;
        i++;
        row++;
        strncpy((char*)&buff[i*LENGTH_OF_LINE],
                            BETWEEN_ROWS,
                            LENGTH_OF_LINE);
        i++;
        if ((st =(board_generate_row(game,
                                    &buff[i*LENGTH_OF_LINE],
                                    row))) != OK)
            return st;
        i++;
        row++;
        strncpy((char*)&buff[i*LENGTH_OF_LINE],
                            BETWEEN_ROWS,
                            LENGTH_OF_LINE);
        i++;
        if ((st =(board_generate_row(game,
                                     &buff[i*LENGTH_OF_LINE],
                                     row))) != OK)
            return st;
        row++;  
    }
    strncpy((char*)&buff[i*LENGTH_OF_LINE],
                            BOARD_EDGE,
                            LENGTH_OF_LINE);
    i++;
    buff[i*LENGTH_OF_LINE] = '\0';
    return OK;
}

status_t board_generate_row(board_t* game,
                            uint8_t* buff,
                            size_t row){
    if (game == NULL || buff == NULL)
        return ERROR_NULL_POINTER;

    size_t i = 0;
    size_t j = 0;

    for (size_t k = 0 ; k < 3 ; k++) {
        buff[i] = 'U';
        i++;
        buff[i] = ' ';
        i++;
        if (game->board[row][j] != 0)
            buff[i] = game->board[row][j] + 48;
        else
            buff[i] = ' ';
        j++;
        i++;
        buff[i] = ' ';
        i++;
        buff[i] = '|';
        i++;
        buff[i] = ' ';
        i++;
        if (game->board[row][j] != 0)
            buff[i] = game->board[row][j] + 48;
        else 
            buff[i] = ' ';
        j++;
        i++;
        buff[i] = ' ';
        i++;
        buff[i] = '|';
        i++;
        buff[i] = ' ';
        i++;
        if (game->board[row][j] != 0)
            buff[i] = game->board[row][j] + 48;
        else
            buff[i] = ' ';
        j++;
        i++;
        buff[i] = ' ';
        i++;        
    }
    buff[i] = 'U';
    i++;
    buff[i] = '\n';

    return OK;
}
