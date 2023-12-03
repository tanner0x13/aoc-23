#include <stdio.h>

#define MAX_LINE_LENGTH 256

int fpeekc(FILE* fp) {
    int c = fgetc(fp);
    ungetc(c, fp);
    return c;
}

void get_next_line(FILE *fp, char *buffer) {
    int i = 0;
    while(!feof(fp) && i < MAX_LINE_LENGTH - 1) {
        int c = fgetc(fp);
        if(c == '\n') { break; }
        buffer[i++] = (char)c;
    }
    buffer[i] = '\0';
}

int main(int argc, char *argv[]) {

    // open input file
    const char *filepath = argv[1];
    FILE *fp = fopen(filepath, "r");
    if(!fp) {
        printf("failed to open input file %s\n", filepath);
        return 1;
    }

    // initialize final sum
    int sum = 0;

    // loop through each line in the input file
    while(fpeekc(fp) != EOF) {
        // extract the next line from the file
        char buffer[MAX_LINE_LENGTH];
        get_next_line(fp, buffer);
        printf("%s\n", buffer);

        // initialize first/last digit - digits must be positive so negative values represent un-set/invalid
        int first_digit = -1;
        int last_digit = -1;

        // loop through string char by char looking for numbers
        char *curr = &buffer[0];
        while(*curr != '\0') {
            if(*curr >= '0' && *curr <= '9') {
                // case 1: the char is actually 0-9
                int val = *curr - '0';
                if(first_digit < 0) { first_digit = val; }
                last_digit = val;
                curr++;
            } else if(0) {
                // case 2: the char is the beginning of a substr spelling out a number (i.e. 'one')

            } else {
                // case 3: nothing to see here, move along
                curr++;
            }
        }

        // combine digits and add to sum
        sum += (10 * first_digit) + last_digit;
    }

    // print answer and return
    printf("Answer: %d\n", sum);
    return 0;
}
