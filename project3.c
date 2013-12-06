#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>

void printMatrix(int *a, int row, int col) {
  int i, j;
  for (i = 0; i < row; ++i) {
    for (j = 0; j < col; ++j)
      printf("%d\t", a[i*col+j]);
    printf("\n");
  }
  return;
}
void find_k_smallest(double *d_matrix, int n, int row, int *iz) {

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
  
}
double distance(double xx, double xy, double yx, double yy) {
  double t1 = yy - xy;
  double t2 = yx - xx;
  return t1 * t1 + t2 * t2;
}







}
