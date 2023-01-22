#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*************************************************************************************************/
static const unsigned int DICT_SIZE = 53;

/*************************************************************************************************/
int item_priority(char ch) {
    int prior = 0;

    if ((ch >= 'a') && (ch <= 'z')) {
        prior = (int)(ch - 'a' + 1);
    } else if ((ch >= 'A') && (ch <= 'Z')) {
        prior = (int)(ch - 'A' + 27);
    }

    return prior;
}

void feed_item_dict(char dict[], char* line, size_t endpos) {
    memset(dict, 0, DICT_SIZE * sizeof(char));

    for (int idx = 0; idx < endpos; idx ++) {
        dict[item_priority(line[idx])] = line[idx];
    }
}

void feed_shared_item_dict(char dict[], char dict0[], char* line, size_t start, size_t endpos) {
    memset(dict, 0, DICT_SIZE * sizeof(char));

    for (int idx = start; idx < endpos; idx ++) {
        int prior = item_priority(line[idx]);

        if (dict0[prior] != '\0') {
            dict[prior] = line[idx];
        }
    }
}

int find_shared_item_dict(char dict0[], char* line, size_t start, size_t endpos) {
    int last_shared_item_prior = 0;

    for (int idx = start; idx < endpos; idx ++) {
        int prior = item_priority(line[idx]);

        if (dict0[prior] != '\0') {
            last_shared_item_prior = prior;
        }
    }

    return last_shared_item_prior;
}

/*************************************************************************************************/
void do_for_rucksack_organization(FILE* in, int n) {
    char* line = NULL;
    size_t capacity = 0;
    ssize_t read;

    char misplaced_dict[DICT_SIZE];
    char **badge_dicts = (char**)malloc(n * sizeof(char**));
    int misplaced_prior_sum = 0;
    int badge_prior_sum = 0;
    int count = 0;

    for (int idx = 0; idx < n; idx ++) {
        badge_dicts[idx] = (char*)malloc(DICT_SIZE * sizeof(char));
    }
    
    while ((read = getline(&line, &capacity, in)) != -1) {
        size_t endpos = read - 1;
        
        /* solve part one */ {
            size_t midpos = endpos / 2;
        
            feed_item_dict(misplaced_dict, line, midpos);
            misplaced_prior_sum += find_shared_item_dict(misplaced_dict, line, midpos, endpos);
        }

        /* solve part two */ {
            int badge_idx = count % n;

            if (badge_idx == 0) {
                feed_item_dict(badge_dicts[badge_idx], line, endpos);
            } else if (badge_idx < n - 1) {
                feed_shared_item_dict(badge_dicts[badge_idx], badge_dicts[badge_idx - 1], line, 0, endpos);
            } else {
                badge_prior_sum += find_shared_item_dict(badge_dicts[badge_idx - 1], line, 0, endpos);
            }

            count ++;
        }
    }

    printf("%d\n", misplaced_prior_sum);
    printf("%d\n", badge_prior_sum);

    /* release memory */ {
        for (int idx = 0; idx < n; idx ++) {
            free(badge_dicts[idx]);
        }
    
        free(badge_dicts);

        if (line != NULL) {
            free(line);
        }
    }
}

/*************************************************************************************************/
int main(int argc, char* argv[]) {
    if (argc > 1) {
        FILE* src = fopen(argv[1], "r");

        if (src != NULL) {
            do_for_rucksack_organization(src, 3);
            fclose(src);
        }
    } else {
        do_for_rucksack_organization(stdin, 3);
    }

    return 0;
}
