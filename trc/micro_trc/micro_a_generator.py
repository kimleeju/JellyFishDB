import random
import string

_LENGTH = 10
_COUNT = 100000
_THREAD_NUM = 2


string_thread = ['1','2']
string_pool = string.digits
string_operation = ['put', 'update' ,'get', 'scan']


result = ""
for j in range(_COUNT) :
	print(string_thread[j%_THREAD_NUM]),
	print(string_operation[0]),
	if(j%_THREAD_NUM == 0) :
		result = ""
		for i in range(_LENGTH) :
			result += random.choice(string_pool)
	print(result)
	
