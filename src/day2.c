#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define MAX_PULLS_PER_GAME 8

typedef struct {
    int red;
    int green;
    int blue;
} pull_t;

typedef struct {
    int gameId;
    int numPulls;
    pull_t pulls[MAX_PULLS_PER_GAME];
} game_t;

void get_next_line(FILE *fp, char *buffer) {
    int i = 0;
    while(!feof(fp) && i < MAX_LINE_LENGTH - 1) {
        int c = fgetc(fp);
        if(c == '\n') { break; }
        buffer[i++] = (char)c;
    }
    buffer[i] = '\0';
}

int fpeekc(FILE *fp) {
    int c = fgetc(fp);
    ungetc(c, fp);
    return c;
}


void tokenize(const char *line, game_t *game) {
    const char *curr = line;
    // parse out game id
    char gameIdStr[4] = {0};  // input has 100\0 games
    curr += 5;  // advance past 'Game ' string
    for(int i = 0; isdigit(*curr); curr++, i++) {
        gameIdStr[i] = *curr;
    }
    curr += 2;  // advance past ': ' string
    game->gameId = atoi(gameIdStr);

    // outer loop to parse out each cube pull
    do {
        // inner loop to parse out each 'number color' combo
        while(1) {
            // read number
            char numberStr[4] = {0};   // assume no 4 digit+ numbers
            for(int i = 0; isdigit(*curr); curr++, i++) {
                numberStr[i] = *curr;
            }
            curr++; // advance past space between number and color

            // read color
            char colorStr[6] = {0};   // green\0 is longest
            for(int i = 0; isalpha(*curr); curr++, i++) {
                colorStr[i] = *curr;
            }

            // write to game struct
            if(strcmp(colorStr, "red") == 0) {
                game->pulls[game->numPulls].red = atoi(numberStr);
            }
            if(strcmp(colorStr, "green") == 0) {
                game->pulls[game->numPulls].green = atoi(numberStr);
            }
            if(strcmp(colorStr, "blue") == 0) {
                game->pulls[game->numPulls].blue = atoi(numberStr);
            }

            if(*curr == ',') {
                curr+= 2;   // advance past ', ' to next number/color pair
            } else if (*curr == ';') {
                curr+= 2;   // advance past '; ' to next number/color pair and break out of inner loop to setup new pull
                break;
            } else if(*curr == '\0') {
                break;      // break out of inner loop but do not advance beyond string bounds
            } else {
                printf("unreachable\n");
            }
        }

        // increment number of pulls parsed in game struct
        game->numPulls++;
    } while(*curr != '\0');
}

void print_game(const game_t *game) {
    printf("id: %d, num pulls: %d\n", game->gameId, game->numPulls);
    for(int i = 0; i < game->numPulls; i++) {
        printf("\tred: %d, green: %d, blue: %d\n",  game->pulls[i].red, game->pulls[i].green, game->pulls[i].blue);
    }
}

int main(int argc, char *argv[]) {

    if(argc != 2) {
        printf("Usage: %s path/to/input\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if(!fp) {
        printf("failed to open input file %s\n", argv[1]);
        return 1;
    }

    int sum = 0;

    while(fpeekc(fp) != EOF) {
        char lineBuf[MAX_LINE_LENGTH];
        get_next_line(fp, lineBuf);
        printf("%s\n", lineBuf);
        game_t game = {0};
        tokenize(lineBuf, &game);
        print_game(&game);
        int redRequired = 0;
        int greenRequired = 0;
        int blueRequired = 0;
        for(int i = 0; i < game.numPulls; i++) {
            if(game.pulls[i].red > redRequired) {
                redRequired = game.pulls[i].red;
            }
            if(game.pulls[i].green > greenRequired) {
                greenRequired = game.pulls[i].green;
            }
            if(game.pulls[i].blue > blueRequired) {
                blueRequired = game.pulls[i].blue;
            }
        }
        sum += redRequired * greenRequired * blueRequired;
    }

    printf("Answer: %d\n", sum);
    fclose(fp);
    return 0;
}
