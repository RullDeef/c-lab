from random import randint
import sys

N = int(sys.argv[1])
with open("perf_test/data.txt", "wt") as file:
    for i in range(N):
        a = randint(-10000, 10000)
        print(a, file=file)
