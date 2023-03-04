#include <iostream>
#include <fstream>

/*************************************************************************************************/
void cat_by_line(std::istream& src) {
    std::string line;

    while (true) {
        std::istream& _ = std::getline(src, line);

        if (!_.fail()) {
            std::cout << line << std::endl;
        } else {
            break;
        }
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
