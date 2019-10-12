/*************************************
 * Lab 3 Exercise 1
 * Name: Jiang Yuxin
 * Student No: A0177868A
 * Lab Group: 11
 *************************************/


#include <pthread.h>
#include <semaphore.h>

typedef struct {
  sem_t mutex;
  int reader_count;
  int writer_count;
  sem_t empty;
} rw_lock;

