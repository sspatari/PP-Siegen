/*************************************************************************
** Implementation of the quicksort algorithm.
**
** Compile:  gcc -pthread -Wall -o qsort qsort.c
**           gcc -pthread -Wall -O -o qsort qsort.c   // with optimization
** Run:      qsort <size-of-array>
** Author:   AK
**
*************************************************************************/

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <cstring>

using namespace std;
const int MINSIZE = 1000000;
struct Args{
	int *a;
	int lo;
	int hi;
	Args(int *a, int lo, int hi) {
		this->a = a;
		this->lo = lo;
		this->hi = hi;
	}
};

void *quicksort(void *args);
void quicksort(int *a, int lo, int hi);
void printArray(int *a, int n);


/*
** Quicksort: sort the elements 'lo' ... 'hi' in array 'a'.
*/

void *quicksort(void *args){
	quicksort(((Args *)args)->a, ((Args *)args)->lo, ((Args *)args)->hi);
}

void quicksort(int *a, int lo, int hi)
{
	int i, j;
	int pivot;

	if (lo >= hi) {
		/*
		** Sub-array is empty or has only one element,
		** i.e., it is already sorted
		*/
		return;
	}

	/* Determine pivot (center element of the array) */
	pivot = a[(lo+hi)/2];

	/* Split aray a into two parts */
	i = lo;
	j = hi;
	while (i <= j) {
		/*
		** Search an element < pivot starting from below
		*/
		while (a[i] < pivot)
			i++;

		/*
		** Search an element > pivot starting from above
		*/
		while (a[j] > pivot)
			j--;

		if (i <= j) {
			/*
			** Swap a[i] and a[j]
			*/
			int tmp = a[i];
			a[i] = a[j];
			a[j] = tmp;

			i++;
			j--;
		}
	}
	/* Recursion */
	int lowerSubArraySize = j - lo + 1;
	if(lowerSubArraySize > MINSIZE) {
		Args args1(a, lo, j);
		pthread_t t1;
		int err;
		do {
			err = pthread_create(&t1, NULL, quicksort, (void *)&args1);
			 if (err != 0) {
		        puts("New Thread Not Created, Retrying....");
		    }
		}while(err != 0);
		quicksort(a, i, hi);
		pthread_join(t1, NULL);
	}else {
		quicksort(a, lo, j);
		quicksort(a, i, hi);
	}
}

/*
** Auxiliary function: returns the current time in
** seconds as a floating point number.
*/
double getTime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec * 0.000001;
}

/*
** Initialize an array 'a' of length 'n' with
** random numbers.
*/
void initialize(int *a, int n)
{
	int i;

	/*
	** Set the start value for the random number generator.
	** Comment this out, if you always want to get the same
	** numbers (for debugging)!
	*/
	//srand((unsigned int)getTime());

	for (i = 0; i < n; i++) {
		a[i] = rand();
	}
}

/*
** Check if an array 'a' of length 'n' is sorted.
** If yes, the result is 1, otherwise the result is 0.
*/
int checkSorted(int *a, int n)
{
	int i;

	for (i = 1; i < n; i++) {
		if (a[i] < a[i-1])
			return 0;
	}
	return 1;
}

/*
** Print an array 'a' of length 'n'.
*/
void printArray(int *a, int n)
{
	int i;

	for (i = 0; i < n; i++) {
		cout << a[i] << " ";
	}
	cout << "\n";
}

/*
** Main program.
** Invocation: qsort <array length>
*/

int compar(const void *a, const void *b)
{
	return ( *(int*)a - *(int*)b );
}

int * arrayDup(int const * src, size_t len)
{
	int * copiedArray = (int *)malloc(len * sizeof(int));
	if(copiedArray == NULL)
	   cout << "Malloc error" << endl;

	// for(int i = 0; i < len ; ++i) {
	//    cout << i << endl;
	//    copiedArray[i] = src[i];
	// }
	memcpy(copiedArray, src, len);
	return copiedArray;
}

int main(int argc, char **argv)
{
	int *array, *array2;
	int n;
	double timeStart, timeEnd;

	if (argc < 2) {
		cerr << "Usage: qsort <array-size>\n";
		exit(1);
	}

	/*
	** 'n' is the command line parameter,
	** converted into an integer number.
	*/
	n = atoi(argv[1]);
	if ((n <= 0) || (n > 200000000)) {
		cerr << "Illegal array size!\n";
		exit(2);
	}

	/*
	** Allocate the array
	*/
	array = new int[n];

	/*
	** Initialisze the array
	*/
	initialize(array, n);
	/*
	** Sort and measure the time ...
	*/

	// printArray(array, n);
	// Copying array to array2;
	array2 = arrayDup(array, n);
	// printArray(array2, n);

	Args args(array, 0, n-1);

	// Our implementation of quicksort
	timeStart = getTime();
	quicksort((void *)&args);
	timeEnd = getTime();

	/*
	** Check it the array is sorted correctly.
	*/

	if (checkSorted(array, n)) {
		cout << "OK, array is sorted!\n";
	}
	else {
		cout << "OOPS, array is NOT sorted!\n";
	}

	cout << "Time for our implementation of sorting: " << setprecision(3) << (timeEnd-timeStart) << " seconds\n";

	// C standard qsort implementation
	timeStart = getTime();
	qsort((void *)array2, n, sizeof(int), compar);
	timeEnd = getTime();

	if (checkSorted(array2, n)) {
		cout << "OK, array is sorted!\n";
	}
	else {
		cout << "OOPS, array is NOT sorted!\n";
	}

	cout << "Time for C standard qsort implementation: " << setprecision(3) << (timeEnd-timeStart) << " seconds\n";

	// printArray(array, n);
	// printArray(array2, n);
	return 0;
}
