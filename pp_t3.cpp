#include <stdio.h>
#include <omp.h>
#include <iostream>

using namespace std;


void swap(int *xp, int *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void compAndSwap(int a[], int i, int j)
{
	if (a[i]>a[j])
		swap(a[i], a[j]);
}

void bitonicSortSecvential(int start, int end, int dir, int *ar)
{
	int counter = 0;
	int noOfElements = end - start + 1;
	for (int j = noOfElements / 2; j > 0; j = j / 2)
	{
		counter = 0;
		for (int i = start; i + j <= end; i++)
		{
			if (counter < j)
			{
				compAndSwap(ar, i, i + j);
				counter++;
			}
			else
			{
				counter = 0;
				i = i + j - 1;
			}
		}
	}
}
void bitonicSortParalel(int a[], int max)
{
	for (int j = 2; j <= max; j = j * 2)
	{
#pragma omp parallel for
		for (int i = 0; i < max; i = i + j)
		{
			if (((i / j) % 2) == 0)
			{
				bitonicSortSecvential(i, i + j - 1, 1, a);
			}
			else
			{
				bitonicSortSecvential(i, i + j - 1, 0, a);
			}
		}
	}
}

void bubbleSort(int arr[], unsigned int n)
{
	unsigned int i, j;
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

int* generareValori(unsigned int N) {
	int* valori = new int[N];
	for (int i = 0; i < N; i++)
		// valori[i] = i;
		// valori[i] = (int) rand();
		valori[i] = N - i;

	return valori;
}

int main(void) {
	const int MAX = 1 << 17; // || MAX = 131072;
	double tStart, tFinal;
	bool success = true;

	int* valoriBubbleSort = generareValori(MAX);
	int* valoriBitonicSort = generareValori(MAX);
	int* valoriBitonicSortParalel = generareValori(MAX);

	cout << endl << "Start test secvential BUBBLE SORT";
	tStart = omp_get_wtime();
	bubbleSort(valoriBubbleSort, MAX);
	tFinal = omp_get_wtime();
	cout << endl << "End test secvential BUBBLE SORT: " << (tFinal - tStart) * 1000 << " ms" << endl;


	cout << endl << "Start test secvential BITONIC SORT ";
	tStart = omp_get_wtime();
	bitonicSortSecvential(0, MAX, 1, valoriBitonicSort);
	tFinal = omp_get_wtime();
	cout << endl << "End test secvential BITONIC SORT: " << (tFinal - tStart) * 1000 << " ms" << endl;


	cout << endl << "Start test paralel BITONIC SORT";
	tStart = omp_get_wtime();
	bitonicSortParalel(valoriBitonicSortParalel, MAX);
	tFinal = omp_get_wtime();
	cout << endl << "End test paralel BITONIC SORT: " << (tFinal - tStart) * 1000 << " ms" << endl;

	for (int i = 0; i < MAX; i++) {
		if (
			valoriBitonicSortParalel[i] != valoriBitonicSort[i]
			|| valoriBubbleSort[i] != valoriBitonicSort[i]
			) {
			cout << endl << "****************Rezultate diferite****************" << endl;
			cout << "Pozitia: " << i << endl;
			cout << "valoriBubbleSort: " << valoriBubbleSort[i] << endl;
			cout << "valoriBitonicSort: " << valoriBitonicSort[i] << endl;
			cout << "valoriBitonicSortParalel: " << valoriBitonicSortParalel[i] << endl;
			success = false;
			break;
		}
	}

	if (success) {
		cout << endl << "****************Rezultate identice****************" << endl;
	}

	return 0;
}
