#include <stdio.h>
#include <stdlib.h>

/*************************************************************************************************/
void find_maximum_calories(FILE* in, int n) {
    char* line = NULL;
    size_t capacity = 0;
    ssize_t read;

    /* `calloc` will do `bzero` */
    long* calories = (long *)calloc(n, sizeof(long));
    long self_cal = 0L;
    long total = 0L;

    while ((read = getline(&line, &capacity, in)) != -1) {
        long cal = strtol(line, NULL, 10);
        
        if (cal > 0L) {
            self_cal += cal;
        } else {
            for (int idx = 0; idx < n; idx ++) {
                if (self_cal > calories[idx]) {
                    calories[idx] = self_cal;
                    break;
                }
            }

            self_cal = 0L;
        }
    }

    /* There is no blank line after the records of last elf */
    for (int idx = 0; idx < n; idx++) {
        if (self_cal > calories[idx]) {
            calories[idx] = self_cal;
            self_cal = 0;
        }

        total += calories[idx];
    }

    if (n > 0) {
        printf("%ld\n", calories[0]);
    }
    
    printf("%ld\n", total);

    /* Don't forget to release the momery */
    free(calories);

    if (line != NULL) {
        free(line);
    }
}

/*************************************************************************************************/
int main(int argc, char* argv[]) {
    if (argc > 1) {
        FILE* src = fopen(argv[1], "r");

        if (src != NULL) {
            find_maximum_calories(src, 3);
            fclose(src);
        }
    } else {
        find_maximum_calories(stdin, 3);
    }

    return 0;
}
