#include <stdio.h>
#include <stdlib.h>

struct heapItem {
  double value;
  int idx;
};

struct Heap {
  struct heapItem *content;
  int capacity;
  int size;
  
};

struct Heap* initHeap(int k) {
  struct Heap *h = (struct Heap*)malloc(sizeof(struct Heap));
  h->capacity = k;
  h->size = 0;
  h->content = (struct heapItem*)malloc(k*sizeof(struct heapItem));
  return h;
}
void freeHeap(struct Heap *h) {
  free(h->content);
  free(h);
  return;
}
void swapItem(struct heapItem *a, struct heapItem *b) {
  struct heapItem tmp;
  tmp = *a;
  *a = *b;
  *b = tmp;
  return;
}

void heapUp(struct Heap *h) {
  int current = h->size;
  while (current > 0) {
    int parent = (current - 1) / 2;
    if (h->content[parent].value < h->content[current].value) {
      swapItem(&h->content[parent], &h->content[current]);
      current = parent;
    }
    else 
      break;
  }
  return;
}

//this heapDown only works for this project, not a general one
void heapDown(struct Heap *h) {
  int current = 0;
  int left = 2*current + 1;
  int right = 2*current + 2;
  while (left < h->size) {
    //consider current, left, right
    if (right < h->size) {
      if (h->content[current].value <= h->content[left].value 
	  && h->content[current].value <= h->content[right].value) {
	swapItem(&h->content[current], &h->content[right]);
	current = right;
	left = 2*current + 1;
	right = 2*current + 2;
      }
      else if (h->content[current].value <= h->content[right].value
	       && h->content[right].value <= h->content[left].value) {
	swapItem(&h->content[current], &h->content[left]);
	current = left;
	left = 2*current + 1;
	right = 2*current + 2;
      }
      else
	break;
    }
    //consider current, left
    else {
      if (h->content[current].value < h->content[left].value) {
	swapItem(&h->content[current], &h->content[left]);
	current = left;
	left = 2*current + 1;
	right = 2*current + 2;
      }
      else
	break;
    }
  }
  return;
}

void insertHeap(struct Heap *h, struct heapItem *item) {
  if (h->size < h->capacity) {
    h->content[h->size] = *item;
    heapUp(h);
    h->size++;
  }
  //the heap is full
  else {
    //it will be inserted only when it is smaller than the root(root will be the biggest one)
    if (item->value < h->content[0].value) {
      h->content[0] = *item;
      heapDown(h);
    }
  }
  return;
}
