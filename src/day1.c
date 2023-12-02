#include <stdio.h>

int main(int argc, char *argv[]) {
    //.TODO: command line input validation
    // open input file
    const char *filepath = argv[1];
    FILE *fp = fopen(filepath, "r");
    if(!fp) {
        printf("failed to open input file %s\n", filepath);
        return 1;
    }

    // initialize
    int sum = 0;    // where the final answer will go
    char first = '\0';  // the first digit in a line
    char last = '\0';   // the last digit in a line

    while(!feof(fp)) {
        // get char
        char c = fgetc(fp);
        // is it a digit?
        if(c >= '0' && c <= '9') {
            // assign `first` only if it has not already been set this line
            if(!first) {
                first = c;
            }
            // keep updating last, whatever is in `last` when \n is reached is the last digit in the line
            last = c;   
        } else if(c == '\n') {
            // line over - turn two digits into an and add to sum
            printf("eol: %c%c\n", first, last);
            sum += (10 * (int)(first - '0')) + (int)(last - '0');
            // reset state for next line
            first = '\0';
            last = '\0';
        }
    }

    // print answer and return
    printf("Answer: %d\n", sum);
    return 0;
}
