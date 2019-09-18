#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void main(int argc, void* argv[]){
	long key_num = atoi(argv[1]);
	long key_max = key_num * 10;
	long * key_box= (long * ) malloc (sizeof(long)* key_num);
	char fname[100]="trc.txt";
	int i,j;

	srand((unsigned) time (NULL));

	for( i=0; i< key_num ; i++){
		key_box[i]= (long) rand() % key_max;
		for(j=0; j < i ; j++){
			if(key_box[j] == key_box[i]){
				i--;
				break;
			}
		}
	}
	for(i=0; i< key_num; i++){
		printf("%ld\n",key_box[i]);
	}
}

