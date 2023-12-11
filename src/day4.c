#include <stdio.h>
#include <stdbool.h>

typedef struct {
    int num;
    int copies;
    bool winning_number_set[100];
} scratcher_t ;


int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s path/to/input\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if(!fp) {
        printf("Could not open %s\n", argv[1]);
        return 1;
    }

    scratcher_t scratchers[256] = {0};
    int total_scratchers_played = 0;

    int index = 0;
    while(!feof(fp)) {
        scratcher_t *scratcher = &scratchers[index];
        total_scratchers_played += 1 + scratcher->copies;
        // scan past 'Card x:' string on each line
        fscanf(fp, "%*s %*d %*c");
        // read each winning numbers
        int winning_num;
        while(fscanf(fp, "%d", &winning_num) > 0) {
            scratcher->winning_number_set[winning_num] = true;
        }
        // discard '|' character
        fscanf(fp, "%*c");
        // read scratcher numbers
        int wins = 0;
        int scratcher_num;
        while(fscanf(fp, "%d", &scratcher_num) > 0) {
            if(scratcher->winning_number_set[scratcher_num]) {
                wins++;
            }
        }
        // add copies to n cards beneath current index where n = `wins`
        for(int i = index+1; i <= index + wins; i++) {
            scratchers[i].copies += 1 + scratcher->copies;
        }
        index++;
    }

    printf("Answer: %d\n", total_scratchers_played);
    return 0;
}
