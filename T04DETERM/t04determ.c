#include <stdio.h>
#include <time.h>
#include <math.h>

#ifdef DEBUG
#include <stdlib.h>
#endif

#define N 101
#define FNAME "Matrix.in"

#ifdef DEBUG
#define lol() fprintf(stderr, "Lol\n");
#else
#define lol()  
#endif

typedef char bool;

double m[N][N];

/*void PrintArr( int n )
{
	int i, j;
	for (i = 0; i < n; i++, printf("\n"))
		for (j = 0; j < n; j++)
			fprintf(stderr, "%.3lf ", m[i][j]);
	fprintf(stderr, "\n");
}*/

void swap( double *a, double *b )
{
	double tmp = *a;
	*a = *b;
	*b = tmp;
}

void SwapW( int n, int a, int b ) /* | | */
{
	int i;
	for (i = 0; i < n; i++)
		swap(&m[i][a], &m[i][b]);
}

void SwapH( int n, int a, int b ) /* --- */
{                                 /* --- */
	int i;
	for (i = 0; i < n; i++)
		swap(&m[a][i], &m[b][i]);
}

void Minus( int n, int a, int b )
{
	int i;
	double mn = m[a][b] / m[b][b];
	for (i = 0; i < n; i++)
		m[a][i] -= mn * m[b][i];
}

int main( void )
{
	int n, i, j, k, mxi, mxj;
	double Ans = 1;
	time_t t;
	bool sgn = 1;
	freopen(FNAME, "rt", stdin);
	scanf("%d", &n);
	t = clock();
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			scanf("%lf", &m[i][j]);
	for (i = 1; i < n; i++)
	{
		mxi = mxj = i;
		for (j = i + 1; j < n; j++)
			for (k = i + 1; k < n; k++)
				if (m[j][k] > m[mxi][mxj] && m[j][k] != 0)
					mxi = j, mxj = k;
		if (mxi != i)
			SwapH(n, mxi, i), sgn *= -1;
		if (mxj != i)
			SwapW(n, mxj, i), sgn *= -1;
		for (j = i; j < n; j++)
		{
			Minus(n, j, i - 1);
		}
	}
	for (i = 0; i < n; i++)
		Ans *= m[i][i];
	printf("Matrix Determination = %lf\n", Ans * sgn);
	fprintf(stderr, "time: %.3f\n", (clock() - t) / (float) CLOCKS_PER_SEC);
	return 0;
}
