#!/usr/bin/python
import sys
import numpy as np
import random

def make_trc_uni(op_num, op_type):

	workload = [x for x in range(op_num)]

	## make load.trc
	random.shuffle(workload)
	fname = "uni_" + op_type + "_load.trc"
	ofile = open(fname, "w")
	if op_type == "put":
		ofile.write("put" + " " + str(0) + "\n")
	else:
		for w in workload:
			ofile.write("put" + " " + str(w) + "\n")

	ofile.close()

	## make run.trc
	random.shuffle(workload)
	fname = "uni_" + op_type + "_run.trc"
	ofile = open(fname, "w")

	for w in workload:
		ofile.write(op_type + " " + str(w) + "\n")

	ofile.close()

def make_trc_zipf(op_num, op_type, skewness):

	workload = np.random.zipf(skewness, op_num)

	## make load.trc
	fname = "zipf_" + str(skewness) + "_" + op_type + "_load.trc"
	ofile = open(fname, "w")

	if op_type == "put":
		ofile.write("put" + " " + str(0) + "\n")
	else:
		for w in workload:
			ofile.write("put" + " " + str(w) + "\n")
	ofile.close()

	## make_run.trc
	random.shuffle(workload)
	fname = "zipf_" + str(skewness) + "_" + op_type + "_run.trc"
	ofile = open(fname, "w")

	for w in workload:
		ofile.write(op_type + " " + str(w) + "\n")

def make_trc_dup(op_num, op_type, degree):

	if op_num % degree != 0:
		print("op_num should be a multiple of degree")
		return;

	sop_num = op_num / degree
	workload = [x for x in range(sop_num)]

	## make load.trc
	random.shuffle(workload)
	fname = "dup_" + str(degree) + "_" + op_type + "_load.trc"
	ofile = open(fname, "w")

	if op_type == "put":
		ofile.write("put" + " " + str(0) + "\n")
	else:
		for n in range(degree):
			for w in workload:
				ofile.write("put" + " " + str(w) + "\n")
			

	ofile.close()

	## make run.trc
	random.shuffle(workload)
	fname = "dup_" + str(degree) + "_" + op_type + "_run.trc"
	ofile = open(fname, "w")

	for n in range(degree):
		for w in workload:
			ofile.write(op_type + " " + str(w) + "\n")

	ofile.close()


def gen():
	# th = int(sys.argv[1])
	op_num = int(sys.argv[1])
	op_types = ["put", "get", "range_query"]

	## dup distribution 
	for op_type in op_types:
		degree = 2
		make_trc_dup(op_num, op_type, degree)
		degree = 4
		make_trc_dup(op_num, op_type, degree)
'''
	## uniform distribution
	for op_type in op_types:
		make_trc_uni(op_num, op_type)

	## zipfian distribution
	for op_type in op_types:
		skewness = 1.2
		while skewness <= 2.0:
			make_trc_zipf(op_num, op_type, skewness)
			skewness += 0.2
'''


if __name__ == "__main__":
	if len(sys.argv) < 2:
		print("Usage: ./.py tot_op")
		print("e.g.,: ./.py 1000")
		exit()
	gen()
