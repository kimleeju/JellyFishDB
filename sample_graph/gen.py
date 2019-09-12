
import random

OPS=100000
MAX=1000
VAR_MAX=5000
VAR_RATIO=3

for i in range(OPS):
	if (i % VAR_RATIO == 0):
		k = random.randint(MAX, VAR_MAX);
	else:
		k = random.randint(1,MAX)
	print(k)


