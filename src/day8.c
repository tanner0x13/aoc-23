#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    char label[4];
    int left_index;
    int right_index;
};

void trim_trailing(char *s) {
    int len = strlen(s);
    int i = len - 1;
    while(isspace(s[i])) {
        s[i] = '\0';
        i--;
    }
}

int get_node_index(const char *s) {
    return (26 * 26 * (s[0] - 'A')) + (26 * (s[1] - 'A')) + (s[2] - 'A');
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

    // read the R/L instructions
    char instructions[512];
    fgets(instructions, 512, fp);
    trim_trailing(instructions);
    const int num_instructions = strlen(instructions);

    // create network
    struct node *network = calloc(26 * 26 * 26, sizeof(struct node));
    int node_index = 0;
    for(int x = 0; x < 26; x++) {
        for(int y = 0; y < 26; y++) {
            for(int z = 0; z < 26; z++) {
                int char_index = 0;
                network[node_index].label[char_index++] = 'A' + x;
                network[node_index].label[char_index++] = 'A' + y;
                network[node_index].label[char_index++] = 'A' + z;
                node_index++;
            }
        }
    }

    // create edges in network based on input
    while(!feof(fp)) {
        char line[64];
        if(fgets(line, 64, fp)) {
            trim_trailing(line);
            if(strlen(line)) {
                const char *delims = " =(,)";
                char *label = strtok(line, delims);
                struct node *n = &network[get_node_index(label)];
                char *left = strtok(NULL, delims);
                n->left_index = get_node_index(left);
                char *right = strtok(NULL, delims);
                n->right_index = get_node_index(right);
            }
        }
    }

    int num_jumps = 0;
    int curr_instruction_index = 0;
    printf("%d\n", num_instructions);
    struct node *current_node = &network[0];
    do {
        printf("%s (jumps completed = %d)\n", current_node->label, num_jumps);
        char instruction = instructions[curr_instruction_index];
        switch (instruction) {
            case 'L': {
                current_node = &network[current_node->left_index];
            } break;
            case 'R': {
                current_node = &network[current_node->right_index];
            } break;
            default: {
                printf("impossible instruction %d\n", (int)instruction);
            } break;
        }
        curr_instruction_index = (curr_instruction_index + 1) % num_instructions;
        num_jumps++;
    } while(strcmp(current_node->label, "ZZZ") != 0);

    printf("Answer: %d\n", num_jumps);
    free(network);
    fclose(fp);
    return 0;
}
