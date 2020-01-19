#include <stdio.h>
#include <omp.h>
#include <iostream>

using namespace std;
//
//static int* kernel(int a[], int p, int q) {
//	int d = 1 << (p - q);
//
//	for (int i = 0; i < (sizeof(a) / sizeof(*a)); i++) {
//		bool up = ((i >> p) & 2) == 0;
//
//		if ((i & d) == 0 && (a[i] > a[i | d]) == up) {
//			int t = a[i];
//			a[i] = a[i | d];
//			a[i | d] = t;
//		}
//	}
//
//	return a;
//}
//
//static void bitonicSort(int MAX, int a[]) {
//	for (int i = 0; i < MAX; i++) {
//		for (int j = 0; j <= i; j++) {
//			
//			kernel(a, i, j);
//		}
//	}
//
//	cout << "ok ";
//
//	for (int k = 0; k < MAX; k++) {
//		cout << k << ": " << a[k] << endl;
//	}
//
//	return;
//}
void compAndSwap(long a[], int i, int j, int dir)
{
	if (dir == (a[i]>a[j]))
		swap(a[i], a[j]);
}

void bitonicMerge(long a[], int low, int cnt, int dir)
{
	if (cnt>1)
	{
		int k = cnt / 2;
		for (int i = low; i<low + k; i++)
			compAndSwap(a, i, i + k, dir);
		bitonicMerge(a, low, k, dir);
		bitonicMerge(a, low + k, k, dir);
	}
}

void bitonicSort(long a[], int low, int cnt, int dir)
{
	if (cnt>1)
	{
		int k = cnt / 2;

		// sort in ascending order since dir here is 1 
		bitonicSort(a, low, k, 1);

		// sort in descending order since dir here is 0 
		bitonicSort(a, low + k, k, 0);

		// Will merge wole sequence in ascending order 
		// since dir=1. 
		bitonicMerge(a, low, cnt, dir);
	}
}

void swap(long *xp, long *yp)
{
	long temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void bubbleSort(long arr[], long n)
{
	int i, j;
	bool swapped = false;
	for (i = 0; i < n - 1; i++)
	{
		swapped = false;
		for (j = 0; j < n - i - 1; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				swap(&arr[j], &arr[j + 1]);
				swapped = true;
			}
		}

		if (swapped == false)
			break;
	}
}

int main(void) {
	const int MAX = 1 << 3;
	//const int MAX = 65536;
	// const int MAX = 131072;

	long a[MAX];
	long p[MAX];

	for (int i = 0; i < MAX; i++) {
		a[i] = p[i] = (int) rand();
	}

	int N = sizeof(a) / sizeof(a[0]);
	int up = 1;   // means sort in ascending order
	int down = 0;   // means sort in ascending order

	// ------


	cout << endl << "Start test secvential - bitonic sort - ";
	double tStart = omp_get_wtime();
	// sort(a, N, up);
	// sort(a, N, down);
	bitonicSort(a, 0, N, down);
	double tFinal = omp_get_wtime();
	cout << endl << "End test secvential - bitonic sort -  in " << (tFinal - tStart) * 1000 << " ms" << endl;

	cout << endl << "Start test secvential - bubble sort -";
	tStart = omp_get_wtime();
	// sort(a, N, up);
	// sort(a, N, down);
	bubbleSort(p, N);
	tFinal = omp_get_wtime();
	cout << endl << "End test secvential in - bubble sort -" << (tFinal - tStart) * 1000 << " ms" << endl;

	// printf("%d\n", N);
	printf("Sorted array: \n");
	for (int i = 0; i<N; i++)
		printf("%d - %d \n", a[i], p[i]);
	return 0;

	return 0;
}