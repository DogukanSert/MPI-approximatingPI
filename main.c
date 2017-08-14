#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include "helper.h"

int main (int argc, char* argv[]){
	MPI_Status s;
	int rank, nop;
	MPI_Init (&argc, &argv);      /* starts MPI */
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id */
	MPI_Comm_size (MPI_COMM_WORLD, &nop);        /* get number of processes */
  	int n = atoi(argv[1]);
  	double start;
	if(rank == 0){
		start = MPI_Wtime();
	}
  	float nums[n];
  	int i;
  	for(i=0;i<n;i++){
  		nums[i] = i* 0.0;
  	}
 	float *randomArr = (float*)malloc(n*sizeof(float));
 	float accumulation;
 	float *filter =  (float*)malloc(n*sizeof(float));
 	
  	randomArr = MPI_Map_Func(nums, n, randomer);

  	filter = MPI_Filter_Func(randomArr, n, hitOrMiss);
 
  	accumulation = MPI_Fold_Func(filter, n, 0.0, accumulator);
  	if(rank==0){
  		printf("input size = %d \t processes = %d\t pi =  %.15f\n",n,nop, (accumulation/n)*4);
  		double end =MPI_Wtime();
		printf("Program took %.5f seconds\n",end-start);
  	}
  	MPI_Barrier(MPI_COMM_WORLD);
  	MPI_Finalize();
	return 0;
}
