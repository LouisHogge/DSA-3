#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

/**
 * 
 * Use of a heap to implementate the priority queue bc
 * Complexity         top      enqueue    dequeue
 * Array              O(n)      O(1)      O(n)
 * LinkedList         O(1)      O(n)      O(1)
 * BinaryHeap         O(1)      O(log n)  O(log n)
 *  
 */

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
 * Shift up the node in order to maintain the heap property.
 *
 * Parameters:
 * -Q     BoundedPriorityQueue to modify
 * -i     the node to shift up
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
  if(Q->size > 0){
    QNode result = Q->Heap[0];
 
    // replace the value at the root with the last leaf
    Q->Heap[0] = Q->Heap[Q->size-1];
    Q->size -= 1;
 
    heapify_down(Q, 0);

    return result;
  }
  else{

  }
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

void printQ(BoundedPriorityQueue*Q);

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
    printf("siz %ld \n", bpq->size);

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

    extract_max(bpq);

    bpqInsert(bpq, key, value);

    // don't heapify after bc assumption that user
    // replace max with priority >= than the old max
  }
}

double bpqMaximumKey(const BoundedPriorityQueue* bpq) {
  if(bpq->size > 0 ){
    return bpq->Heap[0].priority;
  }
  else{
    return 69.420;
  }
} 

size_t* bpqGetItems(const BoundedPriorityQueue* bpq) {
  
  if(bpq->size > 0 ){
    size_t *all_items = malloc(bpq->size * sizeof(size_t));

    BoundedPriorityQueue *Q_tmp = copyQ(bpq);
    printQ(Q_tmp);
    printf("\n");

    size_t it;
    for(size_t i = 0; Q_tmp->size; i++){
      it = extract_max(Q_tmp).value;
      printf("%ld item %ld \n", i, it);
      all_items[i] = it;
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

void printQ(BoundedPriorityQueue*Q){
    for(size_t i = 0; i < Q->size; i++){
        printf("[prio:%f, val:%ld ]", Q->Heap[i].priority, Q->Heap[i].value);
        //printf("[prio:%f]", Q->Heap[i].priority);
    }
    printf("\n");
}

int main(){
  BoundedPriorityQueue *Q = bpqCreate(10);

  bpqInsert(Q,45,0);
  bpqInsert(Q,20,1);
  bpqInsert(Q,14,2);
  bpqInsert(Q,12,3);
  bpqInsert(Q,31,4);
  bpqInsert(Q,7,5);
  bpqInsert(Q,11,6);
  bpqInsert(Q,13,7);
  bpqInsert(Q,7,8);

  // Priority queue before extracting max
  printf("Priority Queue : \n");
  printQ(Q);

  printf("\n");
  printf("------------------------------------------------------------------------------------");
  printf("\n");
  
  QNode n = extract_max(Q);
  // Node with maximum priority
  printf("Node with maximum priority : p:%f, v:%ld \n", n.priority, n.value);

  // Priority queue after extracting max
  printf("Priority queue after extracting maximum : ");
  printQ(Q);
  
  printf("\n");
  printf("------------------------------------------------------------------------------------");
  printf("\n");

  printf("test: ");
  bpqInsert(Q, 3.14,53);
  bpqInsert(Q, 6.28,12);
  printQ(Q);

  BoundedPriorityQueue *Q_bis = copyQ(Q);

  printf("\n");
  printf("------------------------------------------------------------------------------------");
  printf("\n");
  printf("size: %ld \n", Q->size);

  printf("\n");
  printf("------------------------------------------------------------------------------------");
  printf("\n");

  printf("insert %i \n", bpqInsert(Q, 445.55, 54545));

  /*
  QNode n1 = extract_max(Q_bis);
  QNode n2 = extract_max(Q_bis);
  QNode n3 = extract_max(Q_bis);
  QNode n4 = extract_max(Q_bis);
  QNode n5 = extract_max(Q_bis);
  QNode n6 = extract_max(Q_bis);
  QNode n7 = extract_max(Q_bis);
  QNode n8 = extract_max(Q_bis);
  QNode n9 = extract_max(Q_bis);
  QNode n10 = extract_max(Q_bis);
  //QNode n11 = extract_max(Q_bis);
  printf("max : p:%f v:%ld \n", n1.priority, n1.value);
  printf("max : p:%f v:%ld \n", n2.priority, n2.value);
  printf("max : p:%f v:%ld \n", n3.priority, n3.value);
  printf("max : p:%f v:%ld \n", n4.priority, n4.value);
  printf("max : p:%f v:%ld \n", n5.priority, n5.value);
  printf("max : p:%f v:%ld \n", n6.priority, n6.value);
  printf("max : p:%f v:%ld \n", n7.priority, n7.value);
  printf("max : p:%f v:%ld \n", n8.priority, n8.value);
  printf("max : p:%f v:%ld \n", n9.priority, n9.value);
  printf("max : p:%f v:%ld \n", n10.priority, n10.value);
  //printf("max : p:%f v:%ld \n", n11.priority, n11.value);
  */

  size_t size_bis = Q->size;
  QNode *extract_node = malloc(size_bis * sizeof(QNode));
  for(size_t i = 0; i < size_bis; i++ ){
    extract_node[i] = extract_max(Q_bis);
  }
  for (size_t i = 0; i < size_bis; i++){
    printf("max : p:%f v:%ld \n", extract_node[i].priority, extract_node[i].value);
  }

  free(extract_node);
  
  printf("Q_bis after extract \n");
  printQ(Q_bis);
  
  printf("\n");
  printf("------------------------------------------------------------------------------------");
  printf("\n");

  printf("Q after extract of Q_bis \n");
  printQ(Q);

  printf("\n");
  printf("------------------------------------------------------------------------------------");
  printf("\n");

  size_t *item = bpqGetItems(Q);

  printf("bpqGetItems();(all values inside) \n");

  for(size_t i = 0; i < Q->size; i++){
    printf("%ld ", item[i]);
  }
  printf("\n");

  printf("\n");
  printf("----------------------------REPLACEMAXIMUM----------------------------------------");
  printf("\n");

  printQ(Q);
  printf("bpqMaxiumKey = %f \n", bpqMaximumKey(Q));

  printf("\n");

  printf("----------------  replace current max by 1.25 en key et 8 en value ---------------------\n");
  bpqReplaceMaximum(Q, 1.25, 8);
  printQ(Q);
  printf("bpqMaxiumKey = %f \n", bpqMaximumKey(Q));

  printf("\n");

  printf("----------------  replace current max by 2.45 en key et 652 en value ------------------ \n");
  bpqReplaceMaximum(Q, 2.45, 652);
  printQ(Q);
  printf("bpqMaxiumKey = %f \n", bpqMaximumKey(Q));

  printf("\n");


  free(item);
  bpqFree(Q_bis);
  bpqFree(Q);

  return 0;
}
