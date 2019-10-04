/*************************************
 * Lab 3 Exercise 2
 * Name: Jiang Yuxin
 * Student No: A0177868A
 * Lab Group: 11
 *************************************/


#include <pthread.h>

typedef struct {
  pthread_mutex_t mutex;
  int reader_count;
  int writer_count;
  pthread_mutex_t empty;
  pthread_mutex_t writer_mutex;
} rw_lock;

