#include <iostream>
#include <iomanip>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>

using namespace std;


extern double getTime();
extern void * f1(void *arg);
extern void * f2(void *arg);
extern void * f3(void *arg);
extern void * f4(void *arg);
extern void * f5(void *arg);
extern void * f6(void *arg);
extern void * f7(void *arg);

int main(int argc, char **argv)
{
	void *a, *b, *c,  *temp1, *temp2, *temp3;
	double res;
	double start = getTime();
	pthread_t t1, t2, t3;

	a = f1(NULL);
	pthread_create(&t1, NULL, f2, a);
	pthread_create(&t2, NULL, f3, a);
	pthread_join(t1, &b);
	pthread_join(t2, &c);
	a = f4(a);
	double d[2] = {*(double*)b, *(double*)c};
	pthread_create(&t1, NULL, f5, b);
	pthread_create(&t2, NULL, f7, a);
	pthread_create(&t3, NULL, f6, (void*)d);
	pthread_join(t1, &temp1);
	pthread_join(t2, &temp2);
	pthread_join(t3, &temp3);

	res = *(double*)temp1 + *(double*)temp2 + *(double*)temp3;

	cout << "Result = " << setprecision(18) << res << "\n";
	cout << "Time: " << (getTime()-start) << "\n";
	return 0;
}
