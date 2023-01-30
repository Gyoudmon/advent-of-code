#!/usr/bin/env python3

import sys
import io

###################################################################################################
def cp_by_line(src : io.TextIOWrapper, dst : io.TextIOWrapper):
    for line in src:
        print(line, end="", file=dst)



###################################################################################################
def main(argc, argv):
    if argc > 2:
        try:
            src = open(argv[1], 'r')
            dst = open(argv[2], 'w')
            cp_by_line(src, dst)
            src.close()
            dst.close()
        except FileNotFoundError:
            pass

    return 0



###################################################################################################
if __name__ == "__main__":
    sys.exit(main(len(sys.argv), sys.argv))
