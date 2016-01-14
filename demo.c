#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "timer.h"
#include "lab1_IO.h"

const int MAX_THREADS = 64;

/* Global variable:  accessible to all threads */
int thread_count;

int **A;
int **B;
int **C; /* store for results */
int rootp, n;

void Usage(char* prog_name);
void *computeCell(void* rank);  /* Thread function */
int isqrt(unsigned long x);
//int Lab1_loadinput(int ***A, int ***B, int *n);

int isqrt(unsigned long x) {  
/* From http://www.codecodex.com/wiki/Calculate_an_integer_square_root#C */
	 register unsigned long op, res, one;  

	 op = x;  
	 res = 0;  

	 one = 1 << 30;
	 while (one > op) one >>= 2;  

	 while (one != 0) {  
		  if (op >= res + one) {  
				op -= res + one;  
				res += one << 1;  // <-- faster than 2 * one  
		  }  
		  res >>= 1;  
		  one >>= 2;  
	 }  
	 return res;  
}  

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
	long		 thread;  /* Use long in case of a 64-bit system */
	pthread_t* thread_handles; 

	/* Get number of threads from command line */
	if (argc != 2) Usage(argv[0]);
	thread_count = strtol(argv[1], NULL, 10);  
	if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);

	thread_handles = malloc (thread_count*sizeof(pthread_t));

	Lab1_loadinput(*A, *B, int *n);
	rootp = isqrt(thread_count);

	for (thread = 0; thread < thread_count; thread++) {
		pthread_create(&thread_handles[thread], NULL, Hello, (void*) thread);
	}

	for (thread = 0; thread < thread_count; thread++) {
		pthread_join(thread_handles[thread], NULL); 
	}

	Lab1_saveoutput(C, *n, (double) 0);

	free(thread_handles);
	return 0;
}  /* main */

/*-------------------------------------------------------------------*/
void *computeCell(void* rank) { 
	long my_rank = (long) rank;
	int x = my_rank/rootp;
	int y = my_rank%rootp;

	int result;
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			C[i][j] = 0;
			for (k = 0; k < n; k++){
				C[i+x][j+y] += A[i+x][k] * B[k][j+y];
			}
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
