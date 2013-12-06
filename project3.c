#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>
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
double distance(double xx, double xy, double yx, double yy) {
  double t1 = yy - xy;
  double t2 = yx - xx;
  return t1 * t1 + t2 * t2;
}

void find_k_smallest_naive(double *d_matrix, int n, int k, int row, int *iz) {
  int i;
  struct *h = initHeap(k);
  for (i = 0; i < n; i++) {
    struct heapItem *elem = (struct heapItem*)malloc(sizeof(struct heapItem));
    elem->value = d_matrix[row*n+i];
    elem->idx = i;
    insertHeap(h, elem);
  }
  int *idxes = (int*)malloc(k*sizeof(int));
  for (i = 0; i < k; i++) 
    idxes[i] = h->content[i].idx;
  return;
}

void seek(double *a, int n, int k, int *iz) {
  
}

void seek_naive(double *a, int n, int k, int *iz) {
  double *d_matrix = (double*)malloc(n*n*sizeof(double)); //keep track all the distances
  int i, j;

  for (i = 0; i < n; ++i)
    d_matrix[i*n+i] = DBL_MAX;

  for (i = 0; i < n-1; ++i) 
    for (j = i+1; j < n; ++j) {
      double d = distance(a[i], a[n+i], a[j], a[n+j]);
      d_matrix[i*n+j] = d;
      d_matrix[j*n+i] = d;
    }

  for (i = 0; i < n; i++) 
    find_k_smallest_naive(d_matrix, n, k, i, iz);
}

void problem(int n, int k) {
  return;
}
int main() {
  problem(10, 3);
  return 0;
}
