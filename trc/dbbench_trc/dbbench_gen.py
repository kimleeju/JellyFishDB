#!/usr/bin/python
import sys
import numpy as np
import random


def gen(op_type, op_num, threads):
	workload = [x for x in range(op_num)]

	## make load.trc
	random.shuffle(workload)
	fname = "th" + str(threads) + "_" + op_type + "_load.trc"
	ofile = open(fname, "w")

	if op_type == "put":
		ofile.write("put" + " " + str(0) + "\n")
	else:
		n = 0
		while n < threads: 
			k = 0
			for w in workload:
				ofile.write(str(k % threads) + " put" + " " + str(w) + "\n")
				k += 1
			n += 1
			random.shuffle(workload)

	ofile.close()

	## make run.trc
	random.shuffle(workload)
	fname = "th" + str(threads) + "_" + op_type + "_run.trc"
	ofile = open(fname, "w")

	n = 0
	while n < threads:
		k = 0
		for w in workload:
			ofile.write(str(k % threads) + " " + op_type + " " + str(w) + "\n")
			k += 1

		n += 1
		random.shuffle(workload)

	ofile.close()


if __name__ == "__main__":
	if len(sys.argv) < 3:
		print("Usage: ./.py tot_op threads")
		print("e.g.,: ./.py 1000 4")
		exit()

	op_num = int(sys.argv[1])
	op_types = ["put", "get", "range_query"]
	threads = int(sys.argv[2])

	for op_type in op_types:
		gen(op_type, op_num, threads) 
