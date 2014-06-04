#include <stdio.h>
#include <time.h>

#define N 10001

#ifdef DEBUG
#define lol() printf("Lol\n");
#else
#define lol() ;
#endif

typedef char bool;

int pr[N];
double M[N][N], tmp, Ans = 0;

int MergeSort( int *a, int l, int r );

void swap( int *a, int *b )
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void ReverseArr( int *m, int a, int b )
{
	int i;
	for (i = a; i <= a + (b - a) / 2; i++)
		swap(&m[i], &m[b - (i - a)]);
}

void CopyArr( int *a, int *b, int n)
{
	int i;
	for (i = 0; i < n; i++)
		b[i] = a[i];
}

void PrintArr( int *a, int n, int num, int inv )
{
	int i;
	printf("%5d: ", num);
	for (i = 0; i < n; i++)
		printf("%d ", a[i]);
	printf("  -  %2d invertions\n", inv);	
}

void PlusDet( int *p, int n )
{
	int i;
	CopyArr(p, pr, n);
	tmp = 1;
   	for (i = 0; i < n; i++)
   		tmp *= M[i][p[i]];
   	if (MergeSort(pr, 0, n) % 2 == 0)
   		Ans += tmp;
   	else
   		Ans -= tmp;
}

bool NextPerm( int *p, int n )
{
	int i, p1 = -1, mn = -1;
	tmp = 0;
	for (i = n - 1; i > 0; i--)
		if (p[i] > p[i - 1])
		{
			p1 = i - 1;
			break;
		}
	if (p1 == -1)
		return 0;
	for (i = p1 + 1; i < n; i++)
		if (p[i] > p[p1] && (mn == -1 || p[i] < p[mn]))
			mn = i;
   	swap(&p[p1], &p[mn]);
   	ReverseArr(p, p1 + 1, n - 1);

   	PlusDet(p, n);
   	return 1;
}

int MergeSort( int *a, int l, int r )
{
	int ans = 0, m, i, j, k;
	int b[N];
	if (l + 1 == r)
		return 0;

	m = l + (r - l) / 2;
	ans += MergeSort(a, l, m);
	ans += MergeSort(a, m, r);

	i = l, j = m, k = l;
	while (i < m && j < r)
	{
		if (a[i] < a[j])
			b[k++] = a[i++], ans += j - m;
		else
			b[k++] = a[j++];
	}
	while (i < m)
		b[k++] = a[i++], ans += j - m;
	while (j < r)
		b[k++] = a[j++];
	for (i = l; i < r; i++)
		a[i] = b[i];
	return ans;
}

int main( void )
{
	int n, p[N], i, j;
	time_t t = clock();
	freopen("matrix.in", "rt", stdin);
	scanf("%d", &n);
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			scanf("%lf", &M[i][j]);
	for (i = 0; i < n; i++)
		p[i] = i;
	PlusDet(p, n);
	while (NextPerm(p, n));
	printf("Matrix Determination = %lf\n", Ans);
	fprintf(stderr, "time: %.3f\n", (clock() - t) / (float) CLOCKS_PER_SEC);
	return 0;
}
