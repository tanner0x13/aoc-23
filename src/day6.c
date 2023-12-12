#include <stdio.h>

#define MAX_NUM_RACES 16

struct race {
    int time;
    int distance;
};

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s path/to/input\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if(!fp) {
        printf("Failed to open %s\n", argv[1]);
        return 1;
    }

    struct race races[MAX_NUM_RACES];
    int num_races = 0;

    // read in times
    fscanf(fp, "%*s");
    int t;
    while(fscanf(fp, "%d", &t) > 0) {
        printf("%d\n", t);
        races[num_races++].time = t;
    }

    // read in times
    num_races = 0;
    fscanf(fp, "%*s");
    int d;
    while(fscanf(fp, "%d", &d) > 0) {
        printf("%d\n", d);
        races[num_races++].distance = d;
    }

    int product = 1;
    for(int i = 0; i < num_races; i++) {
        struct race *r = &races[i];
        // approach from the left - find minimum time charging
        int t1 = 0;
        while(t1 <= r->time) {
            int velocity = t1;
            int time_remaining = r->time - t1;
            int distance_possible = velocity * time_remaining;
            if(distance_possible > r->distance) {
                break;
            }
            t1++;
        }
        printf("t1 = %d\n", t1);
        // approach from the right - find maximum time charging
        int t2 = r->time;
        while(t2 >= 0) {
            int velocity = t2;
            int time_remaining = r->time - t2;
            int distance_possible = velocity * time_remaining;
            if(distance_possible > r->distance) {
                break;
            }
            t2--;
        }
        printf("t2 = %d\n", t2);

        product *= ((t2 - t1) + 1);
    }


    printf("Answer: %d\n", product);
    fclose(fp);
    return 0;
}
