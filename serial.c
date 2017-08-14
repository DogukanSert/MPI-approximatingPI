#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

int main (int argc, char* argv[]){
	clock_t t;
	t = clock();
	int n = atoi(argv[1]);
	int i;
	float x,y;
	int count = 0;
	for(i=0; i<n; i++){
		x = ((float)rand())/((float)RAND_MAX);
		y = ((float)rand())/((float)RAND_MAX);
		if(sqrt((x*x)+(y*y)) < 1){count++;}
	}
	printf("input size = %d \t pi =  %.15f\n",n, (float)((count*1.0)/n)*4);
	t = clock() - t;
	printf ("Program took %f seconds\n",((float)t)/CLOCKS_PER_SEC);
}
