#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RULES_PER_MAP 100
#define MAX_INPUT_SEEDS 32
#define MAX_MAP_NAME_LENGTH 32

struct map_rule {
    unsigned long source_range_start;
    unsigned long dest_range_start;
    unsigned long range_length;
};
struct map {
    char map_name[MAX_MAP_NAME_LENGTH];
    int num_rules;
    struct map_rule rules[MAX_RULES_PER_MAP];
};
struct node {
    struct map map;
    struct node *next;
};

unsigned long apply_map(const struct map *map, unsigned long input) {
    for(int i = 0; i < map->num_rules; i++) {
        const struct map_rule *rule = &map->rules[i];
        if(input >= rule->source_range_start && input < rule->source_range_start + rule->range_length) {
            long offset = rule->dest_range_start - rule->source_range_start;
            return input + offset;
        }
    }
    return input;
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

    // read the seeds from the input file
    unsigned long seeds[MAX_INPUT_SEEDS];
    int num_seeds = 0;
    {
        fscanf(fp, "%*s");
        unsigned long seed;
        while(fscanf(fp, "%lu", &seed)) {
            seeds[num_seeds++] = seed;
        }
    }

    // read the maps from the input file
    struct node *head = NULL;
    {
        struct node *current = head;
        while(1) {
            char line[128];
            if(!fgets(line, 128, fp)) {
                // eof
                break;
            }

            // new map found - allocate a new node
            struct node *n = malloc(sizeof(struct node));
            memset(n, 0, sizeof(struct node));

            // scan the map name from the line
            sscanf(line, "%s %*s", n->map.map_name);

            // read the map rules
            while(fgets(line, 128, fp)) {
                if(strlen(line) <= 1) {
                    // new line separating each map
                    break;
                }
                unsigned long *a = &n->map.rules[n->map.num_rules].dest_range_start;
                unsigned long *b = &n->map.rules[n->map.num_rules].source_range_start;
                unsigned long *c = &n->map.rules[n->map.num_rules].range_length;
                sscanf(line, "%lu %lu %lu", a, b, c);
                n->map.num_rules++;
            }

            // append to linked list
            if(!current) {
                head = n;
                current = n;
            } else {
                current->next = n;
                current = n;
            }
        }
    }

    // for each input seed, go through mapping chain and keep track of min location id
    unsigned long min_location_id = UINT64_MAX;
    for(int i = 0; i < num_seeds; i++) {
        unsigned long input = seeds[i];
        struct node *current = head;
        while(current) {
            input = apply_map(&current->map, input);
            current = current->next;
        }
        if(input < min_location_id) {
            min_location_id = input;
        }
    }

    printf("Answer: %lu", min_location_id);

    // cleanup and return
    struct node *c = head;
    while(c) {
        struct node *d = c;
        c = c->next;
        free(d);
    }
    fclose(fp);
    return 0;
}
