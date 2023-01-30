#!/usr/bin/env python3

import sys # system-related functions

def main(argc, argv):
    """ The entry point of the application, `main` is not required as the name

    :param argc: argument count
    :param argv: argument vector
    """

    # The first argument is the name of the application
    print("Running: " + argv[0])

    if argc == 2:
        print("  received %s argument from user" % (argc - 1))
    else:
        print("  received %s arguments from user" % (argc - 1))

    # display all arguments line by line
    for i in range(1, argc):
        print("    argv[%s]: %s" % (i, argv[i]))

    return 0



# __name__ is a special variable
# this statement is equivalent to the application launcher in C and C++ 
if __name__ == "__main__":
    sys.exit(main(len(sys.argv), sys.argv))
