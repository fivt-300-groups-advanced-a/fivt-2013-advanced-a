#!/usr/bin/env python3

def main(argv=None):
    import sys, random

    argv = argv or sys.argv

    random.seed(argv[1])
    n = int(argv[2])

    print(n)
    for i in range(n):
        print(random.randint(-10**7, 10**7), random.randint(-10**7, 10**7))


if __name__ == '__main__':
    main()

