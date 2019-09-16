#!/usr/bin/python

import random
import string
import sys
import numpy as np

def gen():
	op = int(sys.argv[1])
	workload = [x for x in range(op)]

	### 1. random write
	random.shuffle(workload)
	ofile = open("rand_w.trc", "w")

	line = 0
	for w in workload:
		ofile.write("put " + str(w) + "\n")
		line += 1

	ofile.close()


	### 2. random read
	random.shuffle(workload)
	ofile = open("rand_r.trc", "w")

	line = 0
	for w in workload:
		ofile.write("put " + str(w) + "\n")
		line += 1

	random.shuffle(workload)

	line = 0
	for w in workload:
		ofile.write("get " + str(w) + "\n")
		line += 1


	### 3. range_query
	random.shuffle(workload)
	ofile = open("rand_rq.trc", "w")

	line = 0
	for w in workload:
		ofile.write("put " + str(w) + "\n")
		line += 1

	random.shuffle(workload)

	line = 0
	for w in workload:
		ofile.write("range_query " + str(w) + " 10" + "\n")
		line += 1

	ofile.close()


if __name__ == "__main__":
	if len(sys.argv) < 2:
		print("Usage: ./.py tot_op")
		print("e.g.,: ./.py 1000")
		exit()
	gen()

'''
	# th = int(sys.argv[1])
	# op = int(sys.argv[2])
	# skew = int(sys.argv[3])
	#
	# workload = [x for x in range(op)]
	# for t in range(th-1):
	# 	tmp = [x for x in range(op)]
	# 	workload = workload + tmp
	#
	# random.shuffle(workload)
_LENGTH = 10
_COUNT = 32000
_THREAD_NUM = 16

string_thread = ['1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16']
string_pool = string.digits
string_operation = ['put', 'update' ,'get', 'scan']
list = []

result = ""
for j in range(_COUNT) :
	result = ""
	for i in range(_LENGTH) :
		result += random.choice(string_pool)
	while result in list : 
		for i in range(_LENGTH) :
			result = ""
			result += random.choice(string_pool)
	list.append(result)
	
for i in range(_COUNT) :
	print(string_thread[i%_THREAD_NUM]),
	print(string_operation[0]),
	print(list[i])

'''
