#include <stdio.h>

/**
 * The entry point of the application, `main` is required as the name
 * @param argc, argument count
 * @param argv, argument vector
 */
int main(int argc, char* argv[]) {
    /* NOTE: the first argument is the name of the application */
    printf("Running: %s\n", argv[0]);
    printf("  received %d argument%s from user:\n", (argc - 1), (argc == 2) ? "" : "s");

    /* display all arguments line by line */
    for (int i = 1; i < argc; i++) {
        printf("    argv[%d]: %s\n", i, argv[i]);
    }
	
    return 0;
}
