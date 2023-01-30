#!/usr/bin/env python3

import sys
import io

###################################################################################################
def cat_by_line(src : io.TextIOWrapper):
    while True:
        line = src.readline()

        if len(line) == 0:
            break
        else:
            print(line, end="")



###################################################################################################
def main(argc, argv):
    if argc > 1:
        for fname in argv[1:]:
            try:
                src = open(fname, 'r')
                cat_by_line(src)
                src.close()
            except FileNotFoundError:
                pass
    else:
        cat_by_line(sys.stdin)

    return 0



###################################################################################################
if __name__ == "__main__":
    sys.exit(main(len(sys.argv), sys.argv))
