#include <iostream>
#include <fstream>
#include <string>
#include <vector>

/*************************************************************************************************/
static void update_max_calories(std::vector<int>& ls, int cal) {
    for (int idx = 0; idx < ls.size(); idx ++) {
        if (ls[idx] < cal) {
            for (int mi = int(ls.size()) - 1; mi > idx; mi --) {
                ls[mi] = ls[mi - 1];
            }

            ls[idx] = cal;
            break;
        }
    }
}

/*************************************************************************************************/
void find_maximum_calorie(std::istream& in) {
    std::string line;
    int max_cal = 0;
    int self_cal = 0;

    while (std::getline(in, line)) {
        if (line.size() > 0) {
            // 空字符串会导致 `stoi` 抛出异常.
            self_cal += std::stoi(line);
        } else {
            if (self_cal > max_cal) {
                max_cal = self_cal;
            }

            self_cal = 0;
        }
    }

    // 最后一条记录之后很可能没有空行
    if (self_cal > max_cal) {
        max_cal = self_cal;
    }

    std::cout << max_cal << std::endl;
}

void find_maximum_calories(std::istream& in, int n) {
    std::string line;
    std::vector<int> calories(n, 0);
    int total = 0;
    int cal = 0;

    while (std::getline(in, line)) {
        if (line.size() > 0) {
            cal += std::stoi(line);
        } else {
            update_max_calories(calories, cal);
            cal = 0;
        }
    }

    // There is no blank line after the records of last elf
    if (cal > 0) {
        update_max_calories(calories, cal);
    }

    for (int i = 0; i < n; i ++) {
        total += calories[i];
    }

    std::cout << total << std::endl;
}

void find_maximum_calories_by_sorting(std::istream& in, int n) {
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

    // There is no blank line after the records of last elf
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
