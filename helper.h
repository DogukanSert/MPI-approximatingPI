#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

float randomer(float x){
	float y = ((float)rand())/((float)RAND_MAX);
	x = ((float)rand())/((float)RAND_MAX);
	return sqrt((x*x)+(y*y));
}
float accumulator(float x,float y){

	return x+y;
}

float hitOrMiss(float x){
	if(x <= 1) return 1.0;
	else return 0.0;
}

float *MPI_Map_Func(float *arr, int size, float (*func)(float)){
	int rank, nop;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id */
 	MPI_Comm_size (MPI_COMM_WORLD, &nop);        /* get number of processes */
 	int low = (rank*size)/nop;
	int high =  floor(((rank+1)*size)/nop);
	int i,j;
	srand(rank);
	float *arr_local = (float*)malloc(size*sizeof(float));
	for(i = low; i < high; i++){
		arr_local[i] = func(arr[i]);
	}

	float *arr_rcv = (float*)malloc(size*sizeof(float));
	for(i = 0; i<nop;i++){	//allreduce
		if(i == rank){
			for(j =0; j<nop; j++){
				if(j != rank){
					MPI_Send(arr_local, size, MPI_FLOAT, j, 0, MPI_COMM_WORLD);
				}
			}
		}else{
			MPI_Recv(arr_rcv, size, MPI_FLOAT, i, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			low = (i*size)/nop;
			high = floor(((i+1)*size)/nop);
			for(j = low; j < high; j++){
				arr_local[j] = arr_rcv[j];
			}
		}
	}
	return arr_local;
}
float MPI_Fold_Func(float *arr, int size, float initial_value, float (*func)(float, float)){
	int rank, nop;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id */
 	MPI_Comm_size (MPI_COMM_WORLD, &nop);        /* get number of processes */
 	int low = (rank*size)/nop;
	int high =  floor(((rank+1)*size)/nop);
	int i,j;
	float accumulation_local = arr[low];
	for(i = low; i < high-1; i++){
		accumulation_local = func(accumulation_local,arr[i+1]);
	}
	float accumulation_rcv;
	float accumulation_final = accumulation_local;
	for(i = 0; i<nop;i++){	//allreduce
		if(i == rank){
			for(j =0; j<nop; j++){
				if(j != rank){
					MPI_Send(&accumulation_local, 1, MPI_FLOAT, j, 0, MPI_COMM_WORLD);
				}
			}
		}else{
			MPI_Recv(&accumulation_rcv, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			accumulation_final = func(accumulation_final,accumulation_rcv);
		}
	}
	return func(initial_value,accumulation_final);
}

float *MPI_Filter_Func(float *arr, int size, float (*pred)(float)){
	int rank, nop;
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id */
 	MPI_Comm_size (MPI_COMM_WORLD, &nop);        /* get number of processes */
 	int low = (rank*size)/nop;
	int high =  floor(((rank+1)*size)/nop);
	int i,j;
	float *arr_local = (float*)malloc(size*sizeof(float));
	for(i = low; i < high; i++){
		arr_local[i] = pred(arr[i]);
	}

	float *arr_rcv = (float*)malloc(size*sizeof(float));
	for(i = 0; i<nop;i++){	//allreduce
		if(i == rank){
			for(j =0; j<nop; j++){
				if(j != rank){
					MPI_Send(arr_local, size, MPI_FLOAT, j, 0, MPI_COMM_WORLD);
				}
			}
		}else{
			MPI_Recv(arr_rcv, size, MPI_FLOAT, i, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			low = (i*size)/nop;
			high = floor(((i+1)*size)/nop);
			for(j = low; j < high; j++){
				arr_local[j] = arr_rcv[j];
			}
		}
	}
	return arr_local;
}

