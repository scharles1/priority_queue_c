#include "pQueue.h"
#include <debug.h>
#include "threads/malloc.h"

// #define TEST
#define CAP_MULT_NUM  3
#define CAP_MULT_DEN  2
/* 
   Module Functions:
*/
static void swap (struct pQueue *pQ, size_t index1, size_t index2);
static void bubble_up (struct pQueue *pQ);
static void bubble_down (struct pQueue *pQ);
static void increase_capacity  (struct pQueue *pQ);

/* Initializes the pQueue object and allocates the array that will be used
   in the maxHeap for implementing the pQueue. This array is at first
   sized to init_capacity but may grow larger if the size of the pQueue
   exceeds this size.
*/
void
pQueue_init (struct pQueue *pQ, size_t init_capacity)
{
  ASSERT (pQ != NULL);

  // allocate the initialize array for the pQueue
  pQ->heap = malloc (sizeof(struct pQueue_elem) * (init_capacity + 1));

  ASSERT (pQ->heap != NULL);

  // set the initial state
  pQ->capacity = init_capacity;
  pQ->size = 0;
}

/* Pushes another element on the priority queue. It does this by copy by taking
   both the priority and the value to be stored and copying them into the
   pQueue.
*/
void 
pQueue_push (struct pQueue *pQ, size_t priority, void *val)
{
  ASSERT (pQ != NULL);

  // need to check if there is enough space and resize if necessary
  if (pQ->size == pQ->capacity)
  {
    increase_capacity (pQ);
  }

  // now add to the back of the pQueue and bubble up
  size_t i = pQ->size + 1;
  pQ->heap[i] = (struct pQueue_elem){priority, val};
  bubble_up(pQ);
  pQ->size++;
}

/* Pops an element off the priority queue and returns it by value to the 
   caller. If the pQueue is empty, an exception is thrown.
*/
struct pQueue_elem
pQueue_pop (struct pQueue *pQ)
{
  ASSERT (pQ != NULL);
  ASSERT (!pQueue_empty(pQ));

  // grab the first element, save it to return, and bubble down
  struct pQueue_elem return_val = pQ->heap[1];
  swap(pQ, 1, pQ->size);
  pQ->size--;
  bubble_down(pQ);

  return return_val;
}

/* Returns the first element on the priority queue by value. This cannot
   be called on an empty queue so it must first be checked to make sure it is
   not empty
*/
struct pQueue_elem
pQueue_peek (struct pQueue *pQ)
{
  ASSERT (pQ != NULL);
  ASSERT (!pQueue_empty(pQ));

  return pQ->heap[1];
}

/* Checks the priority queue to see if it is empty. Returns true if that is
   the case, returns false otherwise.
*/
bool
pQueue_empty (struct pQueue *pQ)
{
  ASSERT (pQ != NULL);

  return pQ->size == 0;
}

/* Cleanup function for the dynamically allocated memory of priority queue
*/
void
pQueue_cleanup (struct pQueue *pQ)
{
  ASSERT (pQ != NULL);

  // just need to free the malloced array
  free(pQ->heap);
}

/* Simply takes to array elements and swaps them
*/
static void 
swap (struct pQueue *pQ, size_t index1, size_t index2)
{
  struct pQueue_elem temp = pQ->heap[index1];
  pQ->heap[index1] = pQ->heap[index2];
  pQ->heap[index2] = temp;
}

/* Performs the bubble_up operation to maintain the heap nature of the
   priority queue after a push
*/
static void 
bubble_up (struct pQueue *pQ)
{
  size_t child = pQ->size + 1;
  size_t parent = child >> 1;

  while(parent)
  {
    if (pQ->heap[child].priority > pQ->heap[parent].priority)
    {
      swap (pQ, child, parent);
    }
    else
    {
      break;
    }

    child = parent;
    parent >>= 1;
  }
}

/* Performs the bubble_down operation to maintain the heap nature of the
   priority queue after a pop
*/
static void 
bubble_down (struct pQueue *pQ)
{
  size_t larger = 1;
  size_t parent, left_child, right_child;

  do
  {
    parent = larger;   
    left_child = parent << 1;
    right_child = (parent << 1) + 1;

    if (left_child <= pQ->size)
      if (pQ->heap[parent].priority < pQ->heap[left_child].priority)
        larger = left_child;
    if (right_child <= pQ->size)
      if (pQ->heap[larger].priority < pQ->heap[right_child].priority)
        larger = right_child; 
    
    if (larger != parent)
      swap (pQ, larger, parent);
  }
  while (parent <= pQ->size && larger != parent);
}

/* Increases the size of the array used for the priority queue using realloc.
   If realloc fails, an assertion is raised.
*/
static void 
increase_capacity  (struct pQueue *pQ)
{
  // increase the capacity of the pQueue
  pQ->heap = realloc(pQ->heap, 
    sizeof(struct pQueue_elem) * ((pQ->capacity * CAP_MULT_NUM) / CAP_MULT_DEN + 1));
  
  ASSERT (pQ->heap != NULL);

  pQ->capacity *= CAP_MULT_NUM;
  pq->capacity /= CAP_MULT_DEN;
}

/* Used for testing purposes
*/
#ifdef TEST

int main (void)
{
  // allocate the initialize queue structure
  struct pQueue pQ;

  // initialize the queue (start small so that it has to reallocate)
  pQueue_init (&pQ, 1);

  // now push a bunch of values onto the priority queue
  for (size_t i = 0; i < 20; i++)
  {
    size_t priority = (size_t)(rand() % 64);
    void *val = (void *)((size_t)rand());
    printf("Pushing val: %p, priority: %zu\r\n", val, priority);
    pQueue_push (&pQ, priority, val);
  }

  // now pop off everything on the stack
  while (!pQueue_empty(&pQ))
  {
    struct pQueue_elem i = pQueue_pop(&pQ);
    printf("Popped val: %p, priority: %zu\r\n", i.value, i.priority);
  }
  pQueue_peek(&pQ);
  return 1;
}

#endif