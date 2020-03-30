#!/usr/bin/env python3

import random
import sys

N = int(sys.argv[1])

file = open("data.txt", "w")
file.write("{}".format(N))

for i in range(N):
  if i % 20 == 0:
    file.write("\n")
  else:
    file.write(" ")
  x = random.randint(-1000, 1000)
  file.write("{}".format(x))

file.close()