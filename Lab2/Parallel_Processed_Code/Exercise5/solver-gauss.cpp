/*************************************************************************
** Iterative solver: Gauss/Seidel method
**
** Author:   RW
**
*************************************************************************/

#include <stdlib.h>
#include <math.h>

#include "cond.h"

/*
** The iterative computation terminates, if the accuracy is at least 'eps'.
*/
extern double eps;


/* Auxiliary Functions *************************************************** */

extern double ** New_Matrix(int m, int n);
extern void Delete_Matrix(double **matrix);


/* Gauss/Seidel relaxation *********************************************** */

/*
** Execute Gau�/Seidel relaxation on the n*n matrix 'a'.
*/
int solver(double **a, int n)
{
	/*
	** Simple estimation for the number of iterations, which is needed to
	** achieve the required accuracy.
	*/
	int kmax = (int)(0.35 / eps);
	int i, j;
	int k;      /* Counts iterations */
	Cond cond(n);

	/*
	** Iterate k times.
	*/
	#pragma omp parallel for private(i, j) schedule(static, 1)
	for (k=0; k<kmax; k++) {
		for (i=1; i<n-1; i++) {
			cond.wait(k, i-1);
			cond.wait(k-1, i+1);
			for (j=1; j<n-1; j++) {
				a[i][j] = 0.25 * (a[i][j-1] + a[i-1][j] +
								  a[i+1][j] + a[i][j+1]);
			}
			cond.signal(k, i);
		}
	}
	return kmax;
}
