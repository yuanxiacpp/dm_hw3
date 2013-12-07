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
  return sqrt(t1 * t1 + t2 * t2);
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
double maxDouble(double a, double b) {
  if (a > b)
    return a;
  return b;
}

void find_k_smallest(double x, double y, int points[], int length, int k, int row, int *iz, double *a, int n) {
  struct Heap *h = initHeap(k);
  int i;
  for (i = 0; i < length; ++i) {
    if (i == row)
      continue;
    struct heapItem *elem = (struct heapItem*)malloc(sizeof(struct heapItem));
    elem->value = distance(x, y, a[i], a[n+i]);
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
void addNewBoxEntry(double boxArray[][4], int row, double x1, double x2, double y1, double y2) {
  boxArray[row][0] = x1;
  boxArray[row][1] = x2;
  boxArray[row][2] = y1;
  boxArray[row][3] = y2;
  return;
}


void addNewActualEntry(int actualArray[][8], int row, int start, int end, int f) {
  int i;
  for (i = 0; i < 8; i++)
    actualArray[row][i] = 0;
  actualArray[row][START] = start;
  actualArray[row][END] = end;
  actualArray[row][FATHER] = f;
  return;
}

double getRadius(double x, double y, double boxArray[][4], int f) {
  double result = distance(x, y, boxArray[f][0], boxArray[f][3]);
  result = maxDouble(result, distance(x, y, boxArray[f][1], boxArray[f][3]));
  result = maxDouble(result, distance(x, y, boxArray[f][0], boxArray[f][2]));
  return maxDouble(result, distance(x, y, boxArray[f][1], boxArray[f][2]));
}


int isInBox(int x, int y, double boxArray[][4], int row) {
  if (x < boxArray[row][0] || x > boxArray[row][1])
    return 0;
  if (y < boxArray[row][2] || y > boxArray[row][3])
    return 0;
  return 1;
}


void quadSplit(int actualArray[][8], double boxArray[][4], int *permutation, int current, int next, double *a, int n) {
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
  

  printf("21\n");
  int i;
  int count = 0;
  while (count < 4) {
    int s = head;
    int e = tail;
    while (s <= e) {
      if (isInBox(a[permutation[s]], a[n+permutation[s]], boxArray, next+count)) 
	s++;
      else {
        swapInt(&permutation[s], &permutation[e]);
	e--;
      }
    }
    //printf("22\n");
    addNewActualEntry(actualArray, next+count, head, s-1, current);
    actualArray[current][CHILDREN+count] = next+count;
    count++;
    head = s;
  }
  return;
}

int intersect(double boxArray[][4], int row, int x, int y, int r) {
  if (isInBox(x, y, boxArray, row))
    return 1;
  if (distance(x, y, boxArray[row][0], boxArray[row][3]) < r ||
      distance(x, y, boxArray[row][1], boxArray[row][3]) < r ||
      distance(x, y, boxArray[row][0], boxArray[row][2]) < r ||
      distance(x, y, boxArray[row][1], boxArray[row][2]) < r)
    return 1;
 
  if (isInBox(x, y+r, boxArray, row) || isInBox(x, y-r, boxArray, row) ||
      isInBox(x-r, y, boxArray, row) || isInBox(x+r, y, boxArray, row))
    return 1;
  return 0;
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
  


  printf("1\n");
  //do quad split
  int current = 0;
  int next = 1;
  while (current < next) {
    //count how many points are there in the section
    if (actualArray[current][END]-actualArray[current][START] > k) {
      quadSplit(actualArray, boxArray, permutation, current, next, a, n);
      next += 4;
    }
    actualArray[current][0] = 1;
    current++;
  }
   printf("1\n");

  
  for (i = 0; i < n; i++) {
    current = 0;
    int first_child_row = actualArray[current][CHILDREN];
    
    //find leaf square for each point
    while (first_child_row != 0) {
      if (isInBox(a[i], a[n+i], boxArray, first_child_row)) 
	current = first_child_row;
      else if (isInBox(a[i], a[n+i], boxArray, first_child_row+1))
	current = first_child_row+1;
      else if (isInBox(a[i], a[n+i], boxArray, first_child_row+2))
	current = first_child_row+2;
      else
	current = first_child_row+3;

      first_child_row = actualArray[current][CHILDREN];
    }
    
    int f = actualArray[current][FATHER];
    double r = getRadius(a[i], a[n+i], boxArray, f);

    int points[BIG_NUMBER];
    int length = 0;
    for (j = actualArray[f][START]; j <= actualArray[f][END]; ++j)
      points[length++] = permutation[j];

    //find all squares that intersect with the circle
    for (j = 0; j < next; ++j) {
      if (actualArray[j][CHILDREN] != 0)
	continue;
      if (intersect(boxArray, j, a[i], a[n+i], r) == 1) {
	int k;
	for (k = actualArray[j][START]; k <= actualArray[j][END]; k++)
	  points[length++] = permutation[k];
      }
    }
    find_k_smallest(a[i], a[n+i], points, length, k, i, iz, a, n);
  }
  
  
 
}


void problem(int n, int k) {
  double *a = (double*)malloc(n*2*sizeof(double));
  int *iz = (int*)malloc(n*k*sizeof(int));
  
  int i, j;
  for (i = 0; i < 2; i++)
    for (j = 0; j < n; j++) 
      a[i*n+j] = (double)rand()/(double)RAND_MAX;

  for (i = 0; i < n; i++)
    for (j = 0; j < k; j++)
      iz[i*k+j] = -1;

  printf("Dots\n");
  printDoubleMatrix(a, 2, n);

  seek(a, n, k, iz);
  printf("Result\n");
  printMatrix(iz, n, k);
}
int main() {
  srand((unsigned)time(NULL));
  problem(10, 1);
  return 0;
}
