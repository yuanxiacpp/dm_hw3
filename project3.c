#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include <time.h>
#include "heap.h"

int BIG_NUMBER = 100000;
int PROCESSED = 0;
int START = 1;
int END = 2;
int FATHER = 3;
int CHILDREN = 4;

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

void swapInt(int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
  return;
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
void addNewBoxEntry(int **boxArray, int row, double x1, double x2, double y1, double y2) {
  boxArray[row][0] = x1;
  boxArray[row][1] = x2;
  boxArray[row][2] = y1;
  boxArray[row][3] = y2;
  return;
}


void addNewActualEntry(int **actualArray, int row, int start, int end, int father) {
  int i;
  for (i = 0; i < 8; i++)
    actualArray[row][i] = 0;
  actualArray[row][START] = start;
  actualArray[row][END] = end;
  actualArray[row][FATHER] = father;
  return;
}

int isInBox(double *a, int n, int i, double **boxArray, int row) {
  int x = a[i];
  int y = a[n+i];
  if (x < boxArray[row][0] || x > boxArray[row][1])
    return 0;
  if (y < boxArray[row][2] || y > boxArray[row][3])
    return 0;
  return 1;
}

void quadSplit(int **actualArray, double **boxArray, int *permutation, int current, int next, double *a, int n) {
  int head = actualArray[current][START];
  int tail = actualArray[current][END];
  double x1 = boxArray[current][0];
  double x2 = boxArray[current][1];
  double y1 = boxArray[current][2];
  double y2 = boxArray[current][3];
  

  addNewBoxEntry(boxArray, next, x1, (x1+x2)/2, (y1+y2)/2, y2);
  addNewBoxEntry(boxArray, next+1, (x1+x2)/2, x2, (y1+y2)/2, y2);
  addNewBoxEntry(boxArray, next+2, x1, (x1+x2)/2, y1, (y1+y2)/2);
  addNewBoxEntry(boxArray, next+3, (x1+x2)/2, x2, y1, (y1+y2)/2);
  
  int i;
  int count = 0;
  while (count < 4) {
    int s = head;
    int e = tail;
    while (s <= e) {
      if (isInBox(a, n, permutation[s], boxArray, next+count)) 
	s++;
      else 
        swapInt(permutation[s], permutation[e--]);
    }
    addNewActualEntry(actualArray, next+count, head, s-1, current);
    actualArray[current][CHILDREN+count] = next+count;
    count++;
    head = s;
  }
  return;
}

void seek(double *a, int n, int k, int *iz) {
  int i, j;
  int *permutation = (int*)malloc(n*sizeof(int));
  for (i = 0; i < n; i++)
    permutation[i] = i; 
  
  double boxArray[BIG_NUMBER][4];
  int actualArray[BIG_NUMBER][8];
 
  addNewActualEntry(actualArray, 0, 0, n-1, -1);
  addNewBoxEntry(boxArray, 0, 0.0, 1.0, 0.0, 1.0);
  
  int current = 0;
  int next = 1;
  while (current < next) {
    //count how many points are there in the section
    if (actualArray[current][START]-actualArray[current][END] > k) {
      quadSplit(actualArray, boxArray, permutation, current, next, a, n);
      next += 4;
    }
    actualArray[current][0] = 1;
    current++;
  }
 
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
