#include <iostream>
#include <math.h>
#include <sys/time.h>

using namespace std;

/* ==================================================
   Thus file should / must not be changed !!!
   ================================================== */

/*
** Auxiliary function: returns the current time in seconds as a floating point value
*/
double getTime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec * 0.000001;
}

/*
** Auxiliary function: calculates for a given time
*/
static void doCompute(double computeTime)
{
	volatile static double glob = 0;
	int i;
	double start = getTime();
	while (getTime() - start < computeTime) {
		for (i=0; i<100000; i++)
			glob += sin(i/10000.0);
	}
}

/*
** Compute intensive functions ...
*/
void * f1(void * args)
{
	cout << "f1 started\n";
	doCompute(1.0);
	cout << "f1 ended\n";
	double *a = new double();
	*a = 0.13456;
	return (void *)a;
}

void * f2(void * args)
{
	cout << "f2 started\n";
	doCompute(1.0);
	cout << "f2 ended\n";
	double *a = new double();
	*a = *(double *)args * 1.1;
	return (void *)a;
}

void * f3(void * args)
{
	cout << "f3 started\n";
	doCompute(1.0);
	cout << "f3 ended\n";
	double *a = new double();
	*a = *(double *)args + 7.65;
	return (void *)a;
}

void * f4(void * args)
{
	cout << "f4 started\n";
	doCompute(1.0);
	cout << "f4 ended\n";
	double *a = new double();
	*a = 999 * (*(double *)args);
	// free(args);
	// args = NULL;
	return (void *)a;
}

void * f5(void * args)
{
	cout << "f5 started\n";
	doCompute(1.0);
	cout << "f5 ended " << "\n";
	double *a = new double();
	*a = 1e20 * (*(double *)args)/0.148016;
	return (void *)a;
}

void * f6(void * args)
{
	cout << "f6 started\n";
	doCompute(1.0);
	cout << "f6 ended\n";
	double *a = new double();
	*a = *(double *)args - 1.1 + *((double *)args + 1);
	return (void *)a;
}

void * f7(void * args)
{
	cout << "f7 started\n";
	doCompute(1.0);
	cout << "f7 ended\n";
	double *a = new double();
	*a = -1e20 + (*(double *)args) * 1.234e8;
	return (void *)a;
}
