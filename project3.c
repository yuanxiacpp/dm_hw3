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

void printBox(double boxArray[][4], int row) {
  int i, j;
  for (i = 0; i < row; ++i) {
    printf("%d:\t", i);
    for (j = 0; j < 4; ++j)
      printf("%6.3f ", boxArray[i][j]);
    printf("\n");
  }
  return;
}
void printActual(int actualArray[][8], int row) {
  int i, j;
  for (i = 0; i < row; ++i) {
    printf("%d:\t", i);
    for (j = 0; j < 8; ++j)
      printf("%4d ", actualArray[i][j]);
    printf("\n");
  }
  return;  
}


void printMatrix(int *a, int row, int col) {
  int i, j;
  for (i = 0; i < row; ++i) {
    for (j = 0; j < col; ++j)
      printf("%d ", a[i*col+j]);
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
    int idx = points[i];
    if (idx == row)
      continue;
    struct heapItem *elem = (struct heapItem*)malloc(sizeof(struct heapItem));
    elem->value = distance(x, y, a[idx], a[n+idx]);
    elem->idx = idx;
    //printf("%d: %f\n", idx, elem->value);
    insertHeap(h, elem);
  }
  //printHeap(h);
  //getchar();

  for (i = k-1; i >= 0; --i)
    iz[row*k+i] = popHeap(h);
  //for (i = 0; i < k; ++i)
  //  printf("%d\t", iz[row*k+i]);
  //printf("\n");
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
  //printHeap(h);
  //getchar();

  for (i = k-1; i >= 0; --i)
    iz[row*k+i] = popHeap(h);
  //for (i = 0; i < k; ++i)
  //  printf("%d\t", iz[row*k+i]);
  //printf("\n");
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
  return;
}
void addNewBoxEntry(double boxArray[][4], int row, double x1, double x2, double y1, double y2) {
  boxArray[row][0] = x1;
  boxArray[row][1] = x2;
  boxArray[row][2] = y1;
  boxArray[row][3] = y2;
  //printf("box entry %d: [%f, %f], [%f, %f]\n", row, x1, x2, y1, y2);
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


int isInBox(double x, double y, double boxArray[][4], int row) {
  //printf("(%f, %f) in range [%f, %f] and [%f, %f]\n",
  //	 x, y, boxArray[row][0], boxArray[row][1],
  //	 boxArray[row][2], boxArray[row][3]);

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
    //printf("row=%d, start=%d, end=%d\n", next+count, head, s-1);
    //getchar();
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
  

  //do quad split
  int current = 0;
  int next = 1;
  while (current < next) {
    //printf("current=%d, next=%d\n", current, next);
    //count how many points are there in the section
    int e = actualArray[current][END];
    int s = actualArray[current][START];
    //printf("e=%d, s=%d\n", e, s);
    if (e-s+1 > k) {
      quadSplit(actualArray, boxArray, permutation, current, next, a, n);
      next += 4;

      
      printf("permutation: \n");
      for (i = 0; i < n; ++i)
	printf("%d ", permutation[i]);
      printf("\nboxArray: \n");
      printBox(boxArray, next);
      printf("actualArray: \n");
      printActual(actualArray, next);

      getchar();
    }
    actualArray[current][0] = 1;
    current++;
  }

  


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

    printf("radius=%f for point %d (%f, %f) in box %d [%f, %f], [%f, %f]\n",
	   r, i, a[i], a[n+i], f, boxArray[f][0], boxArray[f][1], boxArray[f][2], 
    	   boxArray[f][3]);

    //getchar();

    int points[BIG_NUMBER];
    int length = 0;
    for (j = actualArray[f][START]; j <= actualArray[f][END]; ++j)
      points[length++] = permutation[j];

    //find all squares that intersect with the circle
    for (j = 0; j < next; ++j) {
      if (j == f || actualArray[j][CHILDREN] != 0 || actualArray[j][FATHER] == f)
	continue;
      printf("intersect with %d?\n", j);
      if (intersect(boxArray, j, a[i], a[n+i], r) == 1) {
	printf("Yes\n");
	int k;
	for (k = actualArray[j][START]; k <= actualArray[j][END]; k++)
	  points[length++] = permutation[k];
      }
    }

    printf("points included:");
    printMatrix(points, 1, length);
    getchar();

    find_k_smallest(a[i], a[n+i], points, length, k, i, iz, a, n);
  }


  return;
}


void problem(int n, int k) {
  double *a = (double*)malloc(n*2*sizeof(double));
  int *iz_control = (int*)malloc(n*k*sizeof(int));
  int *iz = (int*)malloc(n*k*sizeof(int));
  
  double test[20] = {0.993442, 0.772750, 0.611080, 0.415753, 0.556729, 0.943072, 0.208224, 0.626443, 0.624126, 0.680003, 0.804673, 0.142956, 0.665403, 0.433299, 0.456702, 0.787794, 0.452551, 0.026958, 0.091056, 0.384438};
  
  int i, j;
  //for (i = 0; i < 2; i++)
  //  for (j = 0; j < n; j++) 
  //    a[i*n+j] = (double)rand()/(double)RAND_MAX;

  //for (i = 0; i < n*2; ++i)
  //  printf("%f, ", a[i]);
  //printf("\n");

  for (i = 0; i < 20; i++)
    a[i] = test[i];

  for (i = 0; i < n; i++)
    for (j = 0; j < k; j++) {
      iz[i*k+j] = -1;
      iz_control[i*k+j] = -1;
    }

  printf("Dots\n");
  printDoubleMatrix(a, 2, n);




  seek(a, n, k, iz);

  seek_naive(a, n, k, iz_control);
  printf("Control Result\n");
  printMatrix(iz_control, n, k);

  printf("Test Result\n");
  printMatrix(iz, n, k);

}
int main() {
  srand((unsigned)time(NULL));
  problem(10, 2);
  return 0;
}
