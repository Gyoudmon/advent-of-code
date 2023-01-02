#include <iostream>
#include <fstream>
#include <string>
#include <vector>

/*************************************************************************************************/
void find_maximum_calorie(std::istream& in) {
    std::string line;
    int max_cal = 0;
    int self_cal = 0;

    while (std::getline(in, line)) {
        if (line.size() > 0) {
            self_cal += std::stoi(line);
        } else {
            if (self_cal > max_cal) {
                max_cal = self_cal;
            }

            self_cal = 0;
        }
    }

    if (self_cal > max_cal) {
        max_cal = self_cal;
    }

    std::cout << max_cal << std::endl;
}

void find_maximum_calories(std::istream& in, int n) {
    std::string line;
    std::vector<int> calories;
    int cal = 0;

    while (std::getline(in, line)) {
        if (line.size() > 0) {
            cal += std::stoi(line);
        } else {
            calories.push_back(cal);
            cal = 0;
        }
    }

    if (cal > 0) {
        calories.push_back(cal);
    }

    for (int i = 0; i < n; i ++) {
        for (int j = i + 1; j < calories.size(); j ++) {
            if (calories[i] < calories[j]) {
                cal = calories[i];
                calories[i] = calories[j];
                calories[j] = cal;
            }
        }
    }

    cal = 0;
    for (int i = 0; i < n; i ++) {
        cal += calories[i];
    }

    std::cout << cal << std::endl;
}

/*************************************************************************************************/
int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::ifstream src(argv[1]);

        if (src.is_open()) {
            find_maximum_calories(src, 3);
            src.close();
        }
    } else {
        find_maximum_calorie(std::cin);
    }

    return 0;
}
