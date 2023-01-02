#!/usr/bin/env python3

import sys

def find_maximum_calorie(src):
    max_cal = 0
    self_cal = 0

    for line in src:
        pure_cal = line.strip()

        if len(pure_cal) > 0:
            self_cal += int(pure_cal)
        else:
            if self_cal > max_cal:
                max_cal = self_cal
            self_cal = 0

    print(max(max_cal, self_cal))

def find_maximum_calories(src, n):
    calories = []
    cal = 0

    for line in src:
        pure_cal = line.strip()

        if pure_cal:
            cal += int(pure_cal)
        else:
            calories.append(cal)
            cal = 0

    if (cal > 0):
        calories.append(cal)

    for i in range(0, n):
        for j in range(i + 1, len(calories)):
            if calories[i] < calories[j]:
                cal = calories[i]
                calories[i] = calories[j]
                calories[j] = cal

    print(sum(calories[0:n]))

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
