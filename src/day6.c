#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_NUM_RACES 16

struct race {
    unsigned long time;
    unsigned long distance;
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


    struct race r = {};

    // read in times
    {
        char line[64];
        fgets(line, 64, fp);
        line[strlen(line)-1] = '\0';
        char *pch;
        pch = strtok(line, " ");
        while(pch != NULL) {
            pch = strtok(NULL, " ");
            if(pch) {
                for(int i = 0; i < strlen(pch); i++) {
                    int t = pch[i] - '0';
                    r.time = (r.time * 10) + t;
                }
            }
        }
    }

    // read in distances
    {
        char line[64];
        fgets(line, 64, fp);
        line[strlen(line)-1] = '\0';
        char *pch;
        pch = strtok(line, " ");
        while(pch != NULL) {
            pch = strtok(NULL, " ");
            if(pch) {
                for(int i = 0; i < strlen(pch); i++) {
                    int d = pch[i] - '0';
                    r.distance = (r.distance * 10) + d;
                }
            }
        }
    }

    printf("time: %lu, distance: %lu\n", r.time, r.distance);

        // approach from the left - find minimum time charging
    int t1 = 0;
    while(t1 <= r.time) {
        unsigned long velocity = t1;
        unsigned long time_remaining = r.time - t1;
        unsigned long distance_possible = velocity * time_remaining;
        if(distance_possible > r.distance) {
            break;
        }
        t1++;
    }
    printf("t1 = %d\n", t1);
    // approach from the right - find maximum time charging
    int t2 = r.time;
    while(t2 >= 0) {
        unsigned long velocity = t2;
        unsigned long time_remaining = r.time - t2;
        unsigned long distance_possible = velocity * time_remaining;
        if(distance_possible > r.distance) {
            break;
        }
        t2--;
    }
    printf("t2 = %d\n", t2);

    unsigned long answer = (t2 - t1) + 1;

    printf("Answer: %lu\n", answer);
    fclose(fp);
    return 0;
}
