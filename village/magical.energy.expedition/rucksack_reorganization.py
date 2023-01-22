#!/usr/bin/env python3

import sys
import string

###################################################################################################
def line_group_by_count(lines, n):
    groups = []
    rest = lines

    while True:
        size = len(rest)

        if size == 0:
            break
        elif size < n:
            groups.append(rest)
            break
        else:
            groups.append(rest[0:3])
            rest = rest[3:]

    return groups

def make_item_dict(line, endpos):
    dict = {}

    for ch in line[0:endpos]:
        dict[ch] = True

    return dict

def make_shared_item_dict(dict0, line, start):
    dict = {}
    
    for ch in line[start:]:
        if ch in dict0:
            dict[ch] = True

    return dict

def item_priority(ch):
    prior = 0

    if ch in string.ascii_lowercase:
        prior = ord(ch) - 97 + 1
    elif ch in string.ascii_uppercase:
        prior = ord(ch) - 65 + 27

    return prior

def priority_sum(items):
    return sum(map(item_priority, items))

###################################################################################################
def find_badge_item(lines):
    dict = make_item_dict(lines[0], len(lines[0]))

    for line in lines[1:]:
        dict = make_shared_item_dict(dict, line, 0)

    return list(dict.keys())[0]

def find_misplaced_item(line):
    midpos = len(line) // 2
    dict = make_item_dict(line, midpos)
    shared_dict = make_shared_item_dict(dict, line, midpos)
    
    return list(shared_dict.keys())[0]

###################################################################################################
def do_aoc_datum_from(src):
    lines = []
    
    for line in src:
        lines.append(line.strip())

    print(priority_sum(map(find_misplaced_item, lines)))
    print(priority_sum(map(find_badge_item, line_group_by_count(lines, 3))))

###################################################################################################
def main(argc, argv):
    if argc > 1:
        try:
            with open(argv[1], 'r') as src:
                do_aoc_datum_from(src)
        except FileNotFoundError:
            pass
    else:
        do_aoc_datum_from(sys.stdin)

    return 0



###################################################################################################
if __name__ == "__main__":
    sys.exit(main(len(sys.argv), sys.argv))
