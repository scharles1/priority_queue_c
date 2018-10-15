#ifndef THREADS_PQUEUE_H
#define THREADS_PQUEUE_H

/* Priority Queue.

  An implementation for a priority queue implemented using a max heap. The
  creating thread must allocate the appropriate space (either on it's stack
  or in the heap) for the struct pQueue object. 

  The initializer will allocate an array on the heap which will be used as a 
  max heap for the lifetime of the pQueue. The owner must make sure to call
  pQueue_cleanup before the pQueue object goes out of scope. 

  This priority queue only stores pointers as its elements. 

  The initializer takes a capacity value as an estimate for the size of the 
  pQueue. The user should pick this value based on some estimate of how big
  they believe the queue will grow to. Improper choices may effect effeciency.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Priority Queue element struct. */
struct pQueue_elem
  {
    size_t priority;
    void *value;
  };


/* Priority Queue struct. */
struct pQueue 
  {
    size_t size;
    size_t capacity;
    struct pQueue_elem *heap;
  };

/* Priority Queue initializer */
void pQueue_init (struct pQueue *pQ, size_t init_capacity);

/* Priority Queue methods */
void pQueue_push (struct pQueue *pQ, size_t priority, void *val);
struct pQueue_elem pQueue_pop (struct pQueue *pQ);
struct pQueue_elem pQueue_peek(struct pQueue *pQ);
bool pQueue_empty (struct pQueue *pQ);

/* Priority Queue cleanup */
void pQueue_cleanup (struct pQueue *pQ);


#endif /* threads/pQueue.h */
