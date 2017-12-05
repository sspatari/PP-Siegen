/***************************************************************************
 *  Loop Parallelization
 *
 *  Examine the loops below and check, if they can be parallelized.
 *  If so, parallelize these loops. Then run the program with 4 threads.
 *  It checks whether the result is the same as in the sequential version.
 ***************************************************************************/

/* Do not change the following declarations! */

#include <omp.h>
#include <iostream>
#include <sys/time.h>
#include <iomanip>

using namespace std;

#define N 1000
#define M 100
#define NUMBER_OF_THREADS 4

double a[N], b[N], c[N];
double m[M][M];
int map[N];


/* Start here ... */

double getTime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec * 0.000001;
}

void loop1()
{
	omp_set_num_threads(NUMBER_OF_THREADS);
	#pragma omp parallel for
	for (int i=0; i<N; i++) {
		a[i] = b[i] + c[0];
		b[i] = a[i] - c[i];
	}
}

void loop2()
{
	#pragma omp parallel for ordered
	for (int i=1; i<N; i++) {
		#pragma omp ordered
		a[i] = a[i-1];
		b[i] = a[i] + c[i];
	}
}

void loop3()
{
	double a2[N];
	#pragma omp parallel
	{
		#pragma omp for
		for(int i=3; i<N; i++)
			a2[i] = a[i];
		#pragma omp for
		for (int i=1; i<N-2; i++) {
			a[i] = b[i] + a2[i+2];
			c[i] = b[i-1];
		}
	}
}

void loop4()
{
	// First Method
	double temp_value = a[N/2];
	#pragma omp parallel
	{
		#pragma omp for
		for (int i=0; i<=N/2; i++) {
			a[i] = a[i] - 0.9 * temp_value;
		}
		#pragma omp for
		for (int i=N/2+1; i<N; i++) {
			a[i] = a[i] - 0.9 * a[N/2];
		}
	}

	// // Second Method-better
	// double temp_value1 = a[N/2];
	// a[N/2] = a[N/2] - 0.9 * a[N/2];
	// #pragma omp parallel
	// {
	// 	#pragma omp for nowait
	// 	for (int i=0; i<N/2; i++) {
	// 		a[i] = a[i] - 0.9 * temp_value1;
	// 	}
	// 	#pragma omp for
	// 	for (int i=N/2+1; i<N; i++) {
	// 		a[i] = a[i] - 0.9 * a[N/2];
	// 	}
	// }
}

void loop5()
{
	// double timeStart1, timeEnd1, timeStart2, timeEnd2;
	// timeStart1 = getTime();
	// // First method
	// #pragma omp parallel for ordered
	// for (int i=0; i<N/2; i++) {
	// 	#pragma omp ordered
	// 	a[i+N/3] = (c[i] - a[i])/2;
	// }
	// timeEnd1 = getTime();

	// Second method
	// timeStart2 = getTime();
	#pragma omp parallel
	{
		#pragma omp for
		for (int i=0; i<N/3; i++) {
			a[i+N/3] = (c[i] - a[i])/2;
		}
		#pragma omp for ordered
		for (int i=N/3; i<N/2; i++) {
			#pragma omp ordered
			a[i+N/3] = (c[i] - a[i])/2;
		}
	}
	// timeEnd2 = getTime();

	// cout << "Time for loop: " << setprecision(3) << (timeEnd1-timeStart1) << " seconds\n";
	// cout << "Time for loop: " << setprecision(3) << (timeEnd2-timeStart2) << " seconds\n";
}

void loop6()
{
	#pragma omp parallel for
	for (int i=0; i<N/3; i++) {
		a[i+N/3] = (c[i] - a[i])/2;
	}
}

bool isPowerOfTwo (int x)
{
  /* First x in the below expression is for the case when x is 0 */
  return x && (!(x&(x-1)));
}

void loop7()
{
	#pragma omp parallel for ordered
	for (int i=0; i<N; i++) {
		#pragma omp ordered
		a[map[i]] = a[i] + b[i];
	}
}

void loop8()
{
	for (int i=1; i<M-1; i++) {
		#pragma omp parralel for
		for (int j=1; j<M-1; j++) {
			m[i][j] = (m[i-1][j-1] + m[i-1][j+1] + m[i+1][j-1] + m[i+1][j+1]) / 4;
		}
	}
}
