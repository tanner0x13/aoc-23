#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long get_next_number(long *sequence, int len) {
    int all_zero = 1;
    for(int i = 0; i < len; i++) {
        if(sequence[i] != 0) {
            all_zero = 0;
            break;
        }
    }

    if(all_zero) {
        return 0;
    } else {
        long *new_sequence = malloc(sizeof(long) * (len - 1));
        for(int i = 1; i < len; i++) {
            new_sequence[i-1] = sequence[i] - sequence[i-1];
        }
        long result = sequence[len-1] + get_next_number(new_sequence, len - 1);
        free(new_sequence);
        return result;
    }
}

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

    long sum = 0;

    while(!feof(fp)) {
        char line[256];
        if(fgets(line, 256, fp)) {
            line[strlen(line)-1] = '\0';
            printf("%s", line);
            long sequence[100] = {};
            int len = 0;
            char *tok = strtok(line, " ");
            while(tok) {
                long num = atol(tok);
                sequence[len++] = num;
                tok = strtok(NULL, " ");
            }
            long next = get_next_number(sequence, len);
            printf(" - %ld\n", next);
            sum += next;
        }
    }

    printf("Answer: %ld\n", sum);

    fclose(fp);
    return 0;
}
