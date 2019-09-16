#!/usr/bin/python
import sys
import numpy as np

def gen():
    # th = int(sys.argv[1])
    op = int(sys.argv[1])

    skewness = 1.2
    while skewness <= 2.0:
        fname = "zipf_" + str(skewness) + ".trc"
        ofile = open(fname, "w")

        s = np.random.zipf(skewness, op)
        line = 0
        for x in s:
            ofile.write(str(x) + "\n")
            # ofile.write(str(line % th) + str(x) + "\n")
            line += 1
            # print(x)
        skewness += 0.2
        ofile.close()

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: ./.py tot_op")
        print("e.g.,: ./.py 1000")
        exit()
    gen()
