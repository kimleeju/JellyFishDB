import random
import string

_LENGTH = 10
_COUNT = 100000
_THREAD_NUM = 1


string_thread = ['1']
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
	
