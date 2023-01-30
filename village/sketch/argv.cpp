#include <iostream> /* C++ standard IO stream */

/**
 * The entry point of the application, `main` is required as the name
 * @param argc, argument count
 * @param argv, argument vector
 */
int main(int argc, char* argv[]) {
    /* NOTE: the first argument is the name of the application */
    std::cout << "Running: " << argv[0] << std::endl;
    std::cout << "  received " << (argc - 1)
              << " argument" << ((argc == 2) ? "" : "s")
              << " from user: " << std::endl;
    
    /* display all arguments line by line */
    for (int i = 1; i < argc; i++) {
        std::cout<<"    argv["<< i <<"]: "<<argv[i]<<std::endl;
    }
	
    return 0;
}
