#include <iostream>
#include <fstream>
#include <string>

/*************************************************************************************************/
void cat_by_line(std::istream& in) {
    std::string line;

    while (std::getline(in, line)) {
        std::cout << line << std::endl;
    }
}


/*************************************************************************************************/
int main(int argc, char* argv[]) {
    if (argc > 1) {
        for (int idx = 1; idx < argc; idx++) {
            std::ifstream src(argv[idx]);

            if (src.is_open()) {
                cat_by_line(src);
                src.close();
            }
        }
    } else {
        cat_by_line(std::cin);
    }

    return 0;
}
