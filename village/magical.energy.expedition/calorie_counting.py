#!/usr/bin/env python3

import sys
import io

###################################################################################################
def update_max_calories(calories, cal):
    for idx in range(0, len(calories)):
        if calories[idx] < cal:
            replaced_cal = calories[idx]
            calories[idx] = cal
            update_max_calories(calories, replaced_cal)
            break

###################################################################################################
def find_maximum_calorie(src : io.TextIOWrapper):
    max_cal = 0
    self_cal = 0

    for line in src:
        pure_cal = line.strip()

        if len(pure_cal) > 0:
            # 参数中的任何非数字字符都会导致 `int` 抛出异常
            self_cal += int(pure_cal)
        else:
            if self_cal > max_cal:
                max_cal = self_cal
            self_cal = 0

    # 最后一条记录后面很可能没有空行
    if self_cal > max_cal:
        max_cal = self_cal    

    print(max_cal)

def find_maximum_calories(src, n):
    calories = [0] * n
    cal = 0

    for line in src:
        pure_cal = line.strip()

        if pure_cal:
            cal += int(pure_cal)
        else:
            update_max_calories(calories, cal)
            cal = 0

    if cal > 0:
        update_max_calories(calories, cal)
    
    print(sum(calories))

def find_maximum_calories_by_sorting(src, n):
    calories = []
    cal = 0

    for line in src:
        pure_cal = line.strip()

        if pure_cal:
            cal += int(pure_cal)
        else:
            calories.append(cal)
            cal = 0

    # There is no blank line after the records of last elf
    if (cal > 0):
        calories.append(cal)

    for i in range(0, n):
        for j in range(i + 1, len(calories)):
            if calories[i] < calories[j]:
                cal = calories[i]
                calories[i] = calories[j]
                calories[j] = cal

    print(sum(calories[0:n]))

###################################################################################################
def main(argc, argv):
    if argc > 1:
        try:
            with open(argv[1], 'r') as src:
                find_maximum_calories(src, 3)
        except FileNotFoundError:
            pass
    else:
        find_maximum_calorie(sys.stdin)

    return 0            



###################################################################################################
if __name__ == "__main__":
    sys.exit(main(len(sys.argv), sys.argv))
