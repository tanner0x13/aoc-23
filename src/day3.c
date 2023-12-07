#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// get the character at a given row, col in a board of given dimensions
char get_token_from_board(char *board, int row, int col, int width, int height) {
    if(row < 0 || row >= height || col < 0 || col >= width) {
        return '\0';
    } else {
        return board[(row * width)+col];
    }
}

// checks if character at the given location is a qualifying symbol
int is_symbol(char *board, int row, int col, int width, int height) {
    char c = get_token_from_board(board, row, col, width, height);
    if(isdigit(c) || c == '\0' || c == '.') {
        return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    // check command line args
    if(argc != 2) {
        printf("Usage: %s path/to/input\n", argv[0]);
        return 1;
    }

    // read input file
    FILE *fp = fopen(argv[1], "r");
    if(!fp) {
        printf("Could not open %s\n", argv[1]);
        return 1;
    }

    // determine dimensions
    int width = 0;
    int c;
    while((c = fgetc(fp)) != '\n') {
        width++;
    }
    printf("width: %d\n", width);
    fseek(fp, 0, SEEK_SET);
    int height = 0;
    while(!feof(fp)) {
        if(fgetc(fp) == '\n') {
            height++;
        }
    }
    printf("height: %d\n", height);

    // populate board
    fseek(fp, 0, SEEK_SET);
    char *board = malloc(sizeof(char) * width * height);
    int index = 0;
    for(int row = 0; row < height; row++) {
        for(int col = 0; col < width; col++) {
            board[index++] = fgetc(fp);
        }
        fgetc(fp);
    }

    // print board for to verify
    for(int row = 0; row < height; row++) {
        for(int col = 0; col < width; col++) {
            printf("%c", get_token_from_board(board, row, col, width, height));
        }
        printf("\n");
    }

    int sum = 0;

    // iterate over each char in board looking for numbers
    for(int row = 0; row < height; row++) {
        for(int col = 0; col < width; col++) {
            char c = get_token_from_board(board, row, col, width, height);
            if(isdigit((int)c)) {
                // find full number by walking the row until it ends or not on a digit
                // keep track of bounds for checking adjacent symbols later
                int value = c - '0';
                int left_bound = col;
                int right_bound = col;
                col++;
                while(col < width) {
                    c = get_token_from_board(board, row, col, width, height);
                    if(!isdigit(c)) {
                        break;
                    }
                    value = value * 10 + (c - '0');
                    right_bound = col;
                    col++;
                }
                printf("number found at (%d,%d) = %d\n", row, left_bound, value);

                // check for an adjacent symbol first to the left/right of the bounds
                int can_include = 0;
                if(is_symbol(board, row-1, left_bound-1, width, height) ||      // north-west
                    is_symbol(board, row, left_bound-1, width, height) ||       // west
                    is_symbol(board, row+1, left_bound-1, width, height) ||     // south-west
                    is_symbol(board, row-1, right_bound+1, width, height) ||    // north-east
                    is_symbol(board, row, right_bound+1, width, height) ||      // east
                    is_symbol(board, row+1, right_bound+1, width, height)) {    // south-east
                    can_include = 1;
                }
                // now above and below
                for(int i = left_bound; i <= right_bound; i++) {
                    if(is_symbol(board, row-1, i, width, height) ||             // north
                        is_symbol(board, row+1, i, width, height)) {            // south
                        can_include = 1;
                    }
                }

                // add number to sum if adjacency rule satisfied
                if(can_include) {
                    printf("adjancent symbol found. %d + %d = ", sum, value);
                    sum += value;
                    printf("%d\n", sum);
                }
            }
        }
    }

    // print answer
    printf("Answer: %d\n", sum);

    // cleanup and return
    free(board);
    fclose(fp);
    return 0;
}
