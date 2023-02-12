#include <stdio.h>
#include <stdlib.h>

/*************************************************************************************************/
static inline void backward_calories(long* calories, int n, int idx, long self_cal) {
    for (int mi = n - 1; mi > idx; mi --) {
        calories[mi] = calories[mi - 1];
    }

    calories[idx] = self_cal;
}

void find_maximum_calories(FILE* in, int n) {
    char line[32];
    
    /* `calloc` will do `bzero` */
    long* calories = (long *)calloc(n, sizeof(long));
    long self_cal = 0L;
    long total = 0L;

    while ((fgets(line, sizeof(line) - 1, in)) != NULL) {
        long cal = strtol(line, NULL, 10);
        
        if (cal > 0L) {
            self_cal += cal;
        } else {
            for (int idx = 0; idx < n; idx ++) {
                if (self_cal > calories[idx]) {
                    backward_calories(calories, n, idx, self_cal);
                    break;
                }
            }

            self_cal = 0L;
        }
    }

    /* There is no blank line after the records of last elf */
    for (int idx = 0; idx < n; idx++) {
        if (self_cal > calories[idx]) {
            backward_calories(calories, n, idx, self_cal);
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
#ifndef __windows__
        FILE* src = fopen(argv[1], "r");
#else
        FILE* src;
        
        fopen_s(&src, argv[1], "r");
#endif

        if (src != NULL) {
            find_maximum_calories(src, 3);
            fclose(src);
        }
    } else {
        find_maximum_calories(stdin, 3);
    }

    return 0;
}
