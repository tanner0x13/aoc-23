#include <stdio.h>
#include <stdbool.h>

void set_nth_bit(char *set, int n) {
    set[n] = 1;
}

bool test_nth_bit(const char *set, int n) {
    return set[n] > 0;
}


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

    int total = 0;

    while(!feof(fp)) {
        int card_value = 0;
        char set[100] = {0};
        // scan past 'Card x:' string on each line
        fscanf(fp, "%*s %*d %*c");
        // read each winning numbers
        int winning_num;
        while(fscanf(fp, "%d", &winning_num) > 0) {
            set_nth_bit(set, winning_num);
        }
        // discard '|' character
        fscanf(fp, "%*c");
        // read scratcher numbers
        int scratcher_num;
        while(fscanf(fp, "%d", &scratcher_num) > 0) {
            if(test_nth_bit(set, scratcher_num)) {
                if(card_value) {
                    card_value *= 2;
                } else {
                    card_value = 1;
                }
            }
        }

        total += card_value;
    }

    printf("Answer: %d\n", total);
    return 0;
}
