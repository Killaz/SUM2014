#include <stdio.h>

typedef char bool;

void swap( int *a, int *b )
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void reverse( int *m, int a, int b )
{
	int i;
	for (i = a; i <= a + (b - a) / 2; i++)
		swap(&m[i], &m[b - (i - a)]);
}

bool NextPerm( int *p, int n )
{
	int i, p1 = -1, mn = -1;
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
   	reverse(p, p1 + 1, n - 1);
   	return 1;
}

void PrintArr( int *a, int n )
{
	int i;
	for (i = 0; i < n; i++)
		printf("%d ", a[i]);
	printf("\n");	
}

int main( void )
{
	int n, p[10000], i;
#ifndef DEBUG
	freopen("perm.out", "wt", stdout);
#endif
	scanf("%d", &n);
	for (i = 0; i < n; i++)
		p[i] = i + 1;
	printf("  1: ");
	PrintArr(p, n);
	i = 2;
	while (NextPerm(p, n))
	{
		printf("%3d: ", i++);
		PrintArr(p, n);
	}
	return 0;
}
