#include <stdio.h>
#include <stdlib.h>

struct heapItem {
  double value;
  int idx;
};

struct Heap {
  heapItem *content;
  int capacity;
  int size;
  
};

Heap* initHeap(int k) {
  Heap *h = (Heap*)malloc(sizeof(Heap));
  h->capacity = k;
  h->size = 0;
  h->content = (heapItem*)malloc(k*sizeof(heapItem));
  return h;
}
void freeHeap(Heap *h) {
  free(h->content);
  free(h);
  return;
}

void heapUp(Heap *h) {

}

void heapDown(Heap *h) {

}

void insert(Heap *h, heapItem *item) {
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
