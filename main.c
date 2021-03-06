#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "timer.h"
#include "lab1_IO.h"

const int MAX_THREADS = 64;

int thread_count;

int **A;
int **B;
int **C; /* store for results */
int n;
double start, end;

void Usage(char* prog_name);
void *computeCell(void* rank);  /* Thread function */

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
	long thread;  /* Use long in case of a 64-bit system */
	pthread_t* thread_handles; 
	int i;

	/* Get number of threads from command line */
	if (argc != 2) Usage(argv[0]);
	thread_count = strtol(argv[1], NULL, 10);  
	if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);

	thread_handles = malloc (thread_count*sizeof(pthread_t));

	Lab1_loadinput(&A, &B, &n);

	/*Start Timer*/
	GET_TIME(start);

	C = malloc(n * sizeof(int*));
   	for (i = 0; i < n; i++) {
		C[i] = malloc(n * sizeof(int));
	}

	for (thread = 0; thread < thread_count; thread++) {
		pthread_create(&thread_handles[thread], NULL, computeCell, (void*) thread);
	}

	for (thread = 0; thread < thread_count; thread++) {
		pthread_join(thread_handles[thread], NULL); 
	}

	/*Stop Timer*/
	GET_TIME(end);

	Lab1_saveoutput(C, &n, end - start);

	free(thread_handles);
	return 0;
}  /* main */

/*-------------------------------------------------------------------*/
void *computeCell(void* rank) { 
	long my_rank = (long) rank;

	int a,b,i,j;

	for (i = my_rank; i < n*n; i += thread_count) {
		a = i%n;
		b = i/n;
		C[a][b] = 0;
		for (j = 0; j < n; j++){
			C[a][b] += A[a][j] * B[j][b];
		}
	}

	return NULL;
}

/*-------------------------------------------------------------------*/
void Usage(char* prog_name) {
	fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
	fprintf(stderr, "0 < number of threads <= %d\n", MAX_THREADS);
	exit(0);
}  /* Usage */
