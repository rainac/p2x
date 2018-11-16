#! /usr/bin/env python

import random
import sys

chars = open('chars.txt').read()

NWords = int(1e4)
if len(sys.argv) > 1:
    NWords = int(sys.argv[1])

WLengths = range(10)

for k in range(NWords):
    wlen = random.choice(WLengths)
    w = [' ' for i in range(wlen)]
    for i in range(wlen):
        c = random.choice(chars)
        w[i] = c
    w = "".join(w)
    sys.stdout.write(w + ' ')
