#!/usr/bin/env python3

import sys

def cat_by_line(src):
    for line in src:
        print(line, end="")

def main(argc, argv):
    if argc > 1:
        for fname in argv[1:]:
            try:
                with open(fname, 'r') as src:
                    cat_by_line(src)
            except FileNotFoundError:
                pass
    else:
        cat_by_line(sys.stdin)

    return 0



###################################################################################################
if __name__ == "__main__":
    sys.exit(main(len(sys.argv), sys.argv))
