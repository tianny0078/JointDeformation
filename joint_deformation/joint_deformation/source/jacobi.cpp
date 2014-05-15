//#include "stdafx.h"
#include "jacobi.h"
#include <math.h>
#include <stdio.h>

/*! This static function computes the eigenvalues and
eigenvectors of a SYMMETRIC nxn matrix. This method is used
internally by OpenBabel, but may be useful as a general
eigenvalue finder.

The algorithm uses Jacobi transformations. It is described
e.g. in Wilkinson, Reinsch "Handbook for automatic computation,
Volume II: Linear Algebra", part II, contribution II/1. The
implementation is also similar to the implementation in this
book. This method is adequate to solve the eigenproblem for
small matrices, of size perhaps up to 10x10. For bigger
problems, you might want to resort to the sophisticated routines
of LAPACK.

\note If you plan to find the eigenvalues of a symmetric 3x3
matrix, you will probably prefer to use the more convenient
method findEigenvectorsIfSymmetric()

@param n the size of the matrix that should be diagonalized

@param a array of size n^2 which holds the symmetric matrix
whose eigenvectors are to be computed. The convention is that
the entry in row r and column c is addressed as a[n*r+c] where,
of course, 0 <= r < n and 0 <= c < n. There is no check that the
matrix is actually symmetric. If it is not, the behaviour of
this function is undefined. On return, the matrix is
overwritten with junk.

@param d pointer to a field of at least n doubles which will be
overwritten. On return of this function, the entries d[0]..d[n-1]
will contain the eigenvalues of the matrix.

@param v an array of size n^2 where the eigenvectors will be
stored. On return, the columns of this matrix will contain the
eigenvectors. The eigenvectors are normalized and mutually
orthogonal.
*/

#define TOL 1e-3f
#define MAX_SWEEPS 50

_inline float schurT(float dma, float b);

void jacobi(unsigned int n, float *a, float *d, float *v)
{
	float onorm, dnorm;
	float b, dma, q, t, c, s;
	float atemp, vtemp, dtemp;
	register int i, j, k, l;

	// Set v to the identity matrix, set the vector d to contain the
	// diagonal elements of the matrix a
	d[0] = a[0];
	d[1] = a[4];
	d[2] = a[8];

	for (l = 1; l <= MAX_SWEEPS; l++)
	{
		// Set dnorm to be the maximum norm of the diagonal elements, set
		// onorm to the maximum norm of the off-diagonal elements
		
		dnorm = (float)fabs(d[0]) + (float)fabs(d[1]) + (float)fabs(d[2]);
		onorm = (float)fabs(a[1]) + (float)fabs(a[2]) + (float)fabs(a[5]);
		// Normal end point of this algorithm.
		if((onorm/dnorm) <= TOL)
			goto Exit_now;

		for (j = 1; j < static_cast<int>(n); j++)
		{
			for (i = 0; i <= j - 1; i++)
			{

				b = a[n*i+j];
				if(fabs(b) > 0.0f)
				{
					dma = d[j] - d[i];
					if((fabs(dma) + fabs(b)) <= fabs(dma))
						t = b / dma;
					else
					{
						q = 0.5f * dma / b;
						t = 1.0f/((float)fabs(q) + (float)sqrt(1.0f+q*q));
						if (q < 0.0)
							t = -t;
					}

					c = 1.0f/(float)sqrt(t*t + 1.0f);
					s = t * c;
					a[n*i+j] = 0.0f;

					for (k = 0; k <= i-1; k++)
					{
						atemp = c * a[n*k+i] - s * a[n*k+j];
						a[n*k+j] = s * a[n*k+i] + c * a[n*k+j];
						a[n*k+i] = atemp;
					}

					for (k = i+1; k <= j-1; k++)
					{
						atemp = c * a[n*i+k] - s * a[n*k+j];
						a[n*k+j] = s * a[n*i+k] + c * a[n*k+j];
						a[n*i+k] = atemp;
					}

					for (k = j+1; k < static_cast<int>(n); k++)
					{
						atemp = c * a[n*i+k] - s * a[n*j+k];
						a[n*j+k] = s * a[n*i+k] + c * a[n*j+k];
						a[n*i+k] = atemp;
					}

					for (k = 0; k < static_cast<int>(n); k++)
					{
						vtemp = c * v[n*k+i] - s * v[n*k+j];
						v[n*k+j] = s * v[n*k+i] + c * v[n*k+j];
						v[n*k+i] = vtemp;
					}

					dtemp = c*c*d[i] + s*s*d[j] - 2.0f*c*s*b;
					d[j] = s*s*d[i] + c*c*d[j] + 2.0f*c*s*b;
					d[i] = dtemp;
				} /* end if */
			} /* end for i */
		} /* end for j */
	} /* end for l */

Exit_now:
	//*
	//static int no = 0;
	//no++;
	//if(no % 100 == 0)
	//	printf("%i\n", l-1);
	//numSweeps += l - 1;
	//*/
	return;

	// Now sort the eigenvalues (and the eigenvectors) so that the
	// smallest eigenvalues come first.
	//nrot = l - 1;

	/*
	for (j = 0; j < static_cast<int>(n)-1; j++)
	{
		k = j;
		dtemp = d[k];
		for (i = j+1; i < static_cast<int>(n); i++)
			if(d[i] < dtemp)
			{
				k = i;
				dtemp = d[k];
			}

			if(k > j)
			{
				d[k] = d[j];
				d[j] = dtemp;
				for (i = 0; i < static_cast<int>(n); i++)
				{
					dtemp = v[n*i+k];
					v[n*i+k] = v[n*i+j];
					v[n*i+j] = dtemp;
				}
			}
	}
	*/
}
