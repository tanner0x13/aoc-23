#include <stdio.h>
#include <stdlib.h>

#define NUM_CARDS_PER_HAND 5

enum card {
    JOKER,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    QUEEN,
    KING,
    ACE,
    CARD_COUNT
};

enum rating {
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    FIVE_OF_A_KIND,
    RATING_COUNT
};

struct hand {
    enum card cards[NUM_CARDS_PER_HAND];
    enum rating rating;
    int bid;
};

struct bt_node {
    struct hand hand;
    struct bt_node *left;
    struct bt_node *right;
};

enum card char_to_card(char c) {
    enum card result;
    switch (c) {
        case '2': {
            result = TWO;
        } break;

        case '3': {
            result = THREE;
        } break;

        case '4': {
            result = FOUR;
        } break;

        case '5': {
            result = FIVE;
        } break;

        case '6': {
            result = SIX;
        } break;

        case '7': {
            result = SEVEN;
        } break;

        case '8': {
            result = EIGHT;
        } break;

        case '9': {
            result = NINE;
        } break;

        case 'T': {
            result = TEN;
        } break;

        case 'J': {
            result = JOKER;
        } break;

        case 'Q': {
            result = QUEEN;
        } break;

        case 'K': {
            result = KING;
        } break;

        case 'A': {
            result = ACE;
        } break;

        default: {
            printf("unreachable\n");
        } break;
    }
    return result;
}

char card_to_char(enum card c) {
    enum card result;
    switch (c) {
        case TWO: {
            result = '2';
        } break;

        case THREE: {
            result = '3';
        } break;

        case FOUR: {
            result = '4';
        } break;

        case FIVE: {
            result = '5';
        } break;

        case SIX: {
            result = '6';
        } break;

        case SEVEN: {
            result = '7';
        } break;

        case EIGHT: {
            result = '8';
        } break;

        case NINE: {
            result = '9';
        } break;

        case TEN: {
            result = 'T';
        } break;

        case JOKER: {
            result = 'J';
        } break;

        case QUEEN: {
            result = 'Q';
        } break;

        case KING: {
            result = 'K';
        } break;

        case ACE: {
            result = 'A';
        } break;

        default: {
            printf("unreachable\n");
        } break;
    }
    return result;
}

void set_hand_rating(struct hand *h) {
    int joker_counter = 0;
    int set[CARD_COUNT] = {};
    for(int i = 0; i < NUM_CARDS_PER_HAND; i++) {
        if(h->cards[i] == JOKER) {
            joker_counter++;
        } else {
            int seen = ++set[h->cards[i]];
            switch (seen) {
                case 2: {
                    if(h->rating == HIGH_CARD) {
                        h->rating = ONE_PAIR;
                    } else if(h->rating == ONE_PAIR) {
                        h->rating = TWO_PAIR;
                    } else if(h->rating == THREE_OF_A_KIND) {
                        h->rating = FULL_HOUSE;
                    } else {
                        printf("impossible pairing\n");
                    }
                } break;

                case 3: {
                    if(h->rating == ONE_PAIR) {
                        h->rating = THREE_OF_A_KIND;
                    } else if(h->rating == TWO_PAIR) {
                        h->rating = FULL_HOUSE;
                    } else {
                        printf("impossible trips\n");
                    }
                } break;

                case 4: {
                    if(h->rating == THREE_OF_A_KIND) {
                        h->rating = FOUR_OF_A_KIND;
                    } else {
                        printf("impossible four of a kind\n");
                    }
                } break;

                case 5: {
                    if(h->rating == FOUR_OF_A_KIND) {
                        h->rating = FIVE_OF_A_KIND;
                    } else {
                        printf("impossible five of a kind\n");
                    }
                } break;
            }
        }
    }
    for(int i = 0; i < joker_counter; i++) {
        switch (h->rating) {
            case HIGH_CARD: {
                h->rating = ONE_PAIR;
            } break;

            case ONE_PAIR: {
                h->rating = THREE_OF_A_KIND;
            } break;

            case TWO_PAIR: {
                h->rating = FULL_HOUSE;
            } break;

            case THREE_OF_A_KIND: {
                h->rating = FOUR_OF_A_KIND;
            } break;

            case FOUR_OF_A_KIND: {
                h->rating = FIVE_OF_A_KIND;
            } break;

            default: {
                printf("impossible application of joker\n");
            } break;
        }
    }
}

int cmp_hands(const struct hand *a, const struct hand *b) {
    if(a->rating == b->rating) {
        int i = 0;
        while(a->cards[i] == b->cards[i]) {
            i++;
        }
        return a->cards[i] < b->cards[i] ? -1 : 1;
    } else {
        return a->rating < b->rating ? -1 : 1;
    }
}

void insert(struct bt_node *root, struct bt_node *leaf) {
    int cmp_result = cmp_hands(&leaf->hand, &root->hand);
    if(cmp_result < 0) {
        if(root->left) {
            insert(root->left, leaf);
        } else {
            root->left = leaf;
        }
    } else if(cmp_result > 0) {
        if(root->right) {
            insert(root->right, leaf);
        } else {
            root->right = leaf;
        }
    } else {
        printf("impossible - two hands exactly equal\n");
    }
}

void traverse(struct bt_node *root, int *rank, int *accumulator) {
    if(root->left) {
        traverse(root->left, rank, accumulator);
    }
    for(int i = 0; i < NUM_CARDS_PER_HAND; i++) {
        printf("%c", card_to_char(root->hand.cards[i]));
    }
    int new_rank = *rank + 1;
    int new_accumulator = *accumulator + (new_rank * root->hand.bid);
    printf("\t%d %d\n", new_rank, new_accumulator);
    *rank = new_rank;
    *accumulator = new_accumulator;
    if(root->right) {
        traverse(root->right, rank, accumulator);
    }
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

    struct bt_node *root = NULL;

    while(!feof(fp)) {
        int tmpBid;
        char tmpCards[NUM_CARDS_PER_HAND + 1];
        if(fscanf(fp, "%s %d", tmpCards, &tmpBid) > 0) {
            struct bt_node *node = malloc(sizeof(struct bt_node));
            node->hand.bid = tmpBid;
            for(int i = 0; i < NUM_CARDS_PER_HAND; i++) {
                enum card c = char_to_card(tmpCards[i]);
                node->hand.cards[i] = c;
            }
            set_hand_rating(&node->hand);
            if(!root) {
                root = node;
            } else {
                insert(root, node);
            }
        }
    }

    int rank = 0;
    int accumulator = 0;
    traverse(root, &rank, &accumulator);

    fclose(fp);
    return 0;
}
