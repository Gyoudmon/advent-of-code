#include <iostream>
#include <fstream>

/*************************************************************************************************/
void cp_by_line(std::istream& src, std::ostream& dst) {
    std::string line;
    
    while (std::getline(src, line)) {
        dst << line << std::endl;
    }
}



/*************************************************************************************************/
int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::ifstream src(argv[1]);
        std::ofstream dst(argv[2]);

        if (src.is_open() && dst.is_open()) {
            cp_by_line(src, dst);
        }

        src.close();
        dst.close();
    }

    return 0;
}
