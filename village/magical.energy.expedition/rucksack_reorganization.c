#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*************************************************************************************************/
#define DICT_SIZE 53U

static inline void dict_zero(char dict[]) {
    memset(dict, 0, DICT_SIZE * sizeof(char));
}

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
    dict_zero(dict);

    for (int idx = 0; idx < endpos; idx ++) {
        dict[item_priority(line[idx])] = line[idx];
    }
}

void feed_shared_item_dict(char dict[], char dict0[], char* line, size_t start, size_t endpos) {
    dict_zero(dict);

    for (int idx = (int)(start); idx < (int)(endpos); idx ++) {
        int prior = item_priority(line[idx]);

        if (dict0[prior] != '\0') {
            dict[prior] = line[idx];
        }
    }
}

int find_last_shared_item_prior(char dict0[], char* line, size_t start, size_t endpos) {
    int last_shared_item_prior = 0;

    for (int idx = (int)start; idx < (int)endpos; idx ++) {
        int prior = item_priority(line[idx]);

        if (dict0[prior] != '\0') {
            last_shared_item_prior = prior;
        }
    }

    return last_shared_item_prior;
}

/*************************************************************************************************/
void do_for_rucksack_organization(FILE* in, int n) {
    char line[128];
    
    char misplaced_dict1[DICT_SIZE];
    char misplaced_dict2[DICT_SIZE];
    char **badge_dicts = (char**)malloc((n - 1)* sizeof(char**));
    int misplaced_prior_sum = 0;
    int badge_prior_sum = 0;
    int count = 0;

    for (int idx = 0; idx < n - 1; idx ++) {
        badge_dicts[idx] = (char*)calloc(DICT_SIZE, sizeof(char));
    }

    while ((fgets(line, sizeof(line) - 1, in)) != NULL) {
        size_t endpos = strlen(line) - 1;
        
        /* solve part one */ {
            size_t midpos = endpos / 2;

            dict_zero(misplaced_dict1);
            dict_zero(misplaced_dict2);
        
            for (int idx = 0; idx < midpos; idx ++) {
                if (line[idx] == line[idx + midpos]) {
                    misplaced_prior_sum += item_priority(line[idx]);
                    break;
                } else {
                    int prior1 = item_priority(line[idx]);
                    int prior2 = item_priority(line[idx + midpos]);

                    if (misplaced_dict2[prior1] != '\0') {
                        misplaced_prior_sum += prior1;
                        break;
                    } else if (misplaced_dict1[prior2] != '\0') {
                        misplaced_prior_sum += prior2;
                        break;
                    } else {
                        misplaced_dict1[prior1] = line[idx];
                        misplaced_dict2[prior2] = line[idx + midpos];
                    }
                }
            }
        }

        /* solve part two */ {
            int badge_idx = count % n;

            if (badge_idx == 0) {
                feed_item_dict(badge_dicts[badge_idx], line, endpos);
            } else if (badge_idx < n - 1) {
                feed_shared_item_dict(badge_dicts[badge_idx], badge_dicts[badge_idx - 1], line, 0, endpos);
            } else {
                badge_prior_sum += find_last_shared_item_prior(badge_dicts[badge_idx - 1], line, 0, endpos);
            }

            count ++;
        }
    }

    printf("%d\n", misplaced_prior_sum);
    printf("%d\n", badge_prior_sum);

    /* release memory */ {
        for (int idx = 0; idx < n - 1; idx ++) {
            free(badge_dicts[idx]);
        }
    
        free(badge_dicts);
    }
}

/*************************************************************************************************/
int main(int argc, char* argv[]) {
    if (argc > 1) {
#ifndef __windows__
        FILE* src = fopen(argv[1], "r");
#else
        FILE* src;
        
        fopen_s(&src, argv[1], "r");
#endif

        if (src != NULL) {
            do_for_rucksack_organization(src, 3);
            fclose(src);
        }
    } else {
        do_for_rucksack_organization(stdin, 3);
    }

    return 0;
}
