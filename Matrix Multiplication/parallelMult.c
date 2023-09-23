#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>
#include <math.h>
#include <pthread.h>

#define MAX_VAL 1000
#define MIN_VAL 1
#define MAX_DIM 2000*2000


// 1 Dimensional matrix on stack
double flatA[MAX_DIM];
double flatB[MAX_DIM];

double** randomSquareMatrix(int dimension){
	/*
		Generate 2 dimensional random double matrix.
    For testing execution times Sequential vs Parallel
	*/

	double** matrix = malloc(dimension * sizeof(double*));

	for(int i=0; i<dimension; i++){
		matrix[i] = malloc(dimension * sizeof(double));
	}

	//Random seed
	srandom(time(0)+clock()+random());

  //Parallelizing a repetitive loop for executing test runs
	#pragma omp parallel for
	for(int i=0; i<dimension; i++){
		for(int j=0; j<dimension; j++){
			matrix[i][j] = rand() % MAX_VAL + MIN_VAL;
		}
	}

	return matrix;
}

double** zeroSquareMatrix(int dimension){
	/*
		Generate 2 dimensional zero double matrix.
	*/

	double** matrix = malloc(dimension * sizeof(double*));

	for(int i=0; i<dimension; i++){
		matrix[i] = malloc(dimension * sizeof(double));
	}

	//Random seed
	srandom(time(0)+clock()+random());
	for(int i=0; i<dimension; i++){
		for(int j=0; j<dimension; j++){
			matrix[i][j] = 0;
		}
	}

	return matrix;
}

double sequentialMultiply(double** matrixA, double** matrixB, double** matrixC, int dimension){

  //start, finish times
	struct timeval t0, t1;
	gettimeofday(&t0, 0);

	for(int i=0; i<dimension; i++){
		for(int j=0; j<dimension; j++){
			for(int k=0; k<dimension; k++){
				matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
			}
		}
	}

	gettimeofday(&t1, 0);
	double elapsed = (t1.tv_sec-t0.tv_sec) * 1.0f + (t1.tv_usec - t0.tv_usec) / 1000000.0f;

	return elapsed;
}


void convert(double** matrixA, double** matrixB, int dimension){


	#pragma omp parallel for
	for(int i=0; i<dimension; i++){

		for(int j=0; j<dimension; j++){

			flatA[i * dimension + j] = matrixA[i][j];
			flatB[j * dimension + i] = matrixB[i][j];
		}
	}
}

double optimizedParallelMultiply(double** matrixA, double** matrixB, double** matrixC, int dimension){

  //Offset variables to work on flattened matrices
	int i, j, k, iOff, jOff;
	double tot;

	struct timeval t0, t1;
	gettimeofday(&t0, 0);

  /*
   * The convert function transforms the matrices A and B
   * into 1-dimensional arrays so that they are accesed to
   * easier, optimizing further without the use of parallel
   * processing.
   */
	convert(matrixA, matrixB, dimension);

  /*
   * We share on memory the Resulting matrixC so that
   * it can be used collectively by all the worker threads
   *
   * While the sequence, offset and accumulator variables are
   * private so they are exclusive to the worker threads
   */
	#pragma omp parallel shared(matrixC) private(i, j, k, iOff, jOff, tot)
	{
		#pragma omp for schedule(static)
		for(i=0; i<dimension; i++){

			iOff = i * dimension;

			for(j=0; j<dimension; j++){

				jOff = j * dimension;
				tot = 0;
      
				for(k=0; k<dimension; k++){

					tot += flatA[iOff + k] * flatB[jOff + k];
				}

				matrixC[i][j] = tot;
			}
		}
	}

	gettimeofday(&t1, 0);
	double elapsed = (t1.tv_sec-t0.tv_sec) * 1.0f + (t1.tv_usec - t0.tv_usec) / 1000000.0f;

	return elapsed;
}

int main(){
  
  int dim;
  double res1, res2;

	printf("----------------------------------\n");
	printf("Matrix Multiplication Parallelizing\n");
	printf("----------------------------------\n");

  printf("\n");
	printf("----------------------------------\n");
  printf("Size:  Sequential   vs   Parallel \n");
	printf("----------------------------------\n"); 


  
  for (int i = 1; i <= 10; i++){
    
    dim = pow(2, i);

    double** matrixA = randomSquareMatrix(dim);
    double** matrixB = randomSquareMatrix(dim);
    double** matrixC = zeroSquareMatrix(dim);
    

    res1 = sequentialMultiply(matrixA, matrixB, matrixC, dim);
    
    res2 = optimizedParallelMultiply(matrixA, matrixB, matrixC, dim);
    

    free(matrixC);
    free(matrixA);
    free(matrixB);

    printf("%d:    %f                    %f\n",dim, res1, res2);
  }
  
  return 0;
}
