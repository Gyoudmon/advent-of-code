#include <stdio.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
void cat_by_line(FILE* in) {
    char* line = NULL;
    size_t capacity = 0;
    ssize_t read;

    while ((read = getline(&line, &capacity, in)) != -1) {
        fwrite(line, 1, read, stdout);
    }

    if (line != NULL) {
        free(line);
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
    if (argc > 1) {
        for (int idx = 1; idx < argc; idx ++) {
            FILE* src = fopen(argv[idx], "r");

            if (src != NULL) {
                cat_by_line(src);
                fclose(src);
            }
        }
    } else {
        cat_by_line(stdin);
    }

    return 0;
}
