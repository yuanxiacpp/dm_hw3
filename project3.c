#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include <time.h>
#include "heap.h"

void printMatrix(int *a, int row, int col) {
  int i, j;
  for (i = 0; i < row; ++i) {
    for (j = 0; j < col; ++j)
      printf("%d\t", a[i*col+j]);
    printf("\n");
  }
  return;
}
void printDoubleMatrix(double *a, int row, int col) {
  int i, j;
  for (i = 0; i < row; ++i) {
    for (j = 0; j < col; j++)
      printf("%10.4f", a[i*col+j]);
    printf("\n");
  }
  return;
}
double distance(double xx, double xy, double yx, double yy) {
  double t1 = yy - xy;
  double t2 = yx - xx;
  return t1 * t1 + t2 * t2;
}
int compareInt(const void *a, const void *b) {
  const int *left = a;
  const int *right = b;
  return *left - *right;
}

void find_k_smallest_naive(double *d_matrix, int n, int k, int row, int *iz) {
  int i;
  struct Heap *h = initHeap(k);
  for (i = 0; i < n; i++) {
    if (i == row)
      continue;
    struct heapItem *elem = (struct heapItem*)malloc(sizeof(struct heapItem));
    elem->value = d_matrix[row*n+i];
    elem->idx = i;
    insertHeap(h, elem);
  }
  printHeap(h);
  getchar();

  for (i = k-1; i >= 0; --i)
    iz[row*k+i] = popHeap(h);
  for (i = 0; i < k; ++i)
    printf("%d\t", iz[row*k+i]);
  printf("\n");
  return;
}

void seek(double *a, int n, int k, int *iz) {
  
}

void seek_naive(double *a, int n, int k, int *iz) {
  double *d_matrix = (double*)malloc(n*n*sizeof(double)); //keep track all the distances
  int i, j;

  for (i = 0; i < n; ++i)
    d_matrix[i*n+i] = -1;

  for (i = 0; i < n-1; ++i) 
    for (j = i+1; j < n; ++j) {
      double d = distance(a[i], a[n+i], a[j], a[n+j]);
      d_matrix[i*n+j] = d;
      d_matrix[j*n+i] = d;
    }

  printf("Distances\n");
  printDoubleMatrix(d_matrix, n, n);


  for (i = 0; i < n; i++) 
    find_k_smallest_naive(d_matrix, n, k, i, iz);
}

void problem(int n, int k) {
  double *a = (double*)malloc(n*2*sizeof(double));
  int *iz = (int*)malloc(n*k*sizeof(int));
  
  int i, j;
  for (i = 0; i < 2; i++)
    for (j = 0; j < n; j++) 
      a[i*n+j] = rand() % 20 - 10;

  for (i = 0; i < n; i++)
    for (j = 0; j < k; j++)
      iz[i*k+j] = -1;

  printf("Dots\n");
  printDoubleMatrix(a, 2, n);

  seek_naive(a, n, k, iz);
  printf("Result\n");
  printMatrix(iz, n, k);
}
int main() {
  srand((unsigned)time(NULL));
  problem(10, 5);
  return 0;
}
