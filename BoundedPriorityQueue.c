#include <stdlib.h>
#include <stddef.h>

#include "BoundedPriorityQueue.h"

typedef struct Q_node_t{
    double priority;
    size_t value;
}QNode;

struct bounded_priority_queue_t {
  QNode *Heap;
  size_t size;
  size_t capacity;
};

/*--------------------------------------------------------------------------- *
 * Swap two elements in an array.
 * 
 * Parameters:
 * -array        The array where the elements will be exchange
 * -firstIndex   The adress of the first value to swap
 * -secondIndex    The adress of the second value to swap
 * 
 *---------------------------------------------------------------------------- */
static void swap(QNode *H, size_t firstIndex, size_t secondIndex);

/*--------------------------------------------------------------------------- *
 * Return the index of the parent node from the node i.
 *
 * Parameters:
 * -i      Node 
 *
 * Return
 * -parent   Index of the parent node
 *--------------------------------------------------------------------------- */
static size_t parent(size_t i);

/*--------------------------------------------------------------------------- *
 * Return the index of the left son from the node i.
 *
 * Parameters:
 * -i      Node 
 *
 * Return
 * -left   Index of the left son
 *--------------------------------------------------------------------------- */
static size_t left_child(size_t i);
 
/*--------------------------------------------------------------------------- *
 * Return the index of the right son from the node i.
 *
 * Parameters:
 * -i      Node
 *
 * Return
 * -right  Index of the right son
 *--------------------------------------------------------------------------- */
static size_t right_child(size_t i);


/*--------------------------------------------------------------------------- *
 * Shift up the node in order to maintain the heap property.
 *
 * Parameters:
 * -Q     BoundedPriorityQueue to modify
 * -i     the node to shift up
 *--------------------------------------------------------------------------- */
static void heapify_up(BoundedPriorityQueue *Q, size_t i);

/*--------------------------------------------------------------------------- *
 * Shift down the node in order to maintain the heap property.
 *
 * Parameters:
 * -Q     BoundedPriorityQueue to modify
 * -i     the node to shift down
 *--------------------------------------------------------------------------- */
static void heapify_down(BoundedPriorityQueue *Q, size_t i);

/*--------------------------------------------------------------------------- *
 * Exrtract the QNode with the priority max and maintain the heap property.
 * 
 * Parameters:
 * -Q      BoundedPriorityQueue from which extract the node
 *
 * Return
 * -result  the QNode at the top of the heap
 *--------------------------------------------------------------------------- */
static QNode extract_max(BoundedPriorityQueue *Q);

/*--------------------------------------------------------------------------- *
 * Make a copy from a priority queue and all elemnts insert there
 * (can be free with bpqfree)
 * Parameters:
 * -Q      BoundedPriorityQueue to copy
 *
 * Return
 * -Q_copy   Copy of the priority queue
 *--------------------------------------------------------------------------- */
static BoundedPriorityQueue *copyQ(const BoundedPriorityQueue *Q);

static void swap(QNode *Heap, size_t firstIndex, size_t secondIndex){
    QNode tmp = Heap[firstIndex];
    Heap[firstIndex] = Heap[secondIndex];
    Heap[secondIndex] = tmp;
}

static size_t parent(size_t i){
    return (i - 1) / 2;
}

static size_t left_child(size_t i){
    return ((2 * i) + 1);
}

static size_t right_child(size_t i){
    return ((2 * i) + 2);
}

static void heapify_up(BoundedPriorityQueue *Q, size_t i){
  while (i > 0 && Q->Heap[parent(i)].priority < Q->Heap[i].priority) {
    // swap parent and current node
    swap(Q->Heap, parent(i), i);

    // update i to parent of i
    i = parent(i);
  }
}

static void heapify_down(BoundedPriorityQueue *Q, size_t root){
    size_t largest = root;
 
    // left Child
    size_t left = left_child(root);
 
    if (left <= Q->size && Q->Heap[largest].priority < Q->Heap[left].priority) {
      largest = left;
    }
 
    // right Child
    size_t right = right_child(root);
 
    if (right <= Q->size && Q->Heap[right].priority > Q->Heap[largest].priority) {
      largest = right;
    }
 
    // if root not same as largest
    if (root != largest) {
        swap(Q->Heap, root, largest);
        heapify_down(Q, largest);
    }
}

static QNode extract_max(BoundedPriorityQueue *Q){
  QNode result = Q->Heap[0];

  // replace the value at the root with the last leaf
  Q->Heap[0] = Q->Heap[Q->size-1];
  Q->size -= 1;

  heapify_down(Q, 0);

  return result;
}

static BoundedPriorityQueue *copyQ(const BoundedPriorityQueue *Q){

  BoundedPriorityQueue *Q_copy = malloc(sizeof(BoundedPriorityQueue));

  QNode *H_copy = malloc(Q->capacity * sizeof(QNode));

  for(size_t i = 0; i < Q->size; i++){
    H_copy[i].priority = Q->Heap[i].priority;
    H_copy[i].value = Q->Heap[i].value;
  }

  Q_copy->Heap = H_copy; 
  Q_copy->capacity = Q->capacity;
  Q_copy->size = Q->size;

  return Q_copy;
}

BoundedPriorityQueue* bpqCreate(size_t capacity) {

  BoundedPriorityQueue *Q = malloc(sizeof(BoundedPriorityQueue));

  QNode *Heap = malloc(capacity * sizeof(QNode));

  Q->capacity = capacity;
  Q->Heap = Heap;
  Q->size = 0;

  return Q;
}

void bpqFree(BoundedPriorityQueue* bpq) {
  if(bpq->Heap != NULL){
    free(bpq->Heap);
  }

  if(bpq != NULL){
    free(bpq);
  }
}

bool bpqInsert(BoundedPriorityQueue* bpq, double key, size_t value) {
  if(bpq->size != bpq->capacity){
    QNode new_node;
    new_node.priority = key;
    new_node.value = value; 

    bpq->Heap[bpq->size] = new_node;

    bpq->size += 1;

    // heapify up to maintain heap property
    heapify_up(bpq, bpq->size-1);

    return true;
  }
  else{
    return false;
  }
}

void bpqReplaceMaximum(BoundedPriorityQueue* bpq, double key, size_t value) {
  if(bpq->size != 0){

    // delete the maximum
    extract_max(bpq);

    // place another pair key/value in the right position in the bpq
    bpqInsert(bpq, key, value);
  }
}

double bpqMaximumKey(const BoundedPriorityQueue* bpq) {
  if(bpq->size > 0 ){
    return bpq->Heap[0].priority;
  }
  else{
    // undefined behavior but I have to return sth
    return 69.420;
  }
} 

size_t* bpqGetItems(const BoundedPriorityQueue* bpq) {
  
  if(bpq->size > 0 ){
    size_t *all_items = malloc(bpq->size * sizeof(size_t));

    // do a copy just to extract all QNode without affecting original bpq
    BoundedPriorityQueue *Q_tmp = copyQ(bpq);
    
    for(size_t i = 0; Q_tmp->size; i++){
      all_items[i] = extract_max(Q_tmp).value;
    }

    bpqFree(Q_tmp);

    return all_items;
  }
  else{
    return NULL;// no item to get
  }
}

size_t bpqSize(const BoundedPriorityQueue* bpq) {
  return bpq->size;
}

size_t bpqCapacity(const BoundedPriorityQueue* bpq) {
  return bpq->capacity;
}
