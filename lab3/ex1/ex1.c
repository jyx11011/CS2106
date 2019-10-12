
/*************************************
 * Lab 3 Exercise 1
 * Name: Jiang Yuxin
 * Student No: A0177868A
 * Lab Group: 11
 *************************************/
#include <pthread.h>
#include <semaphore.h>
#include "rw_lock.h"

void initialise(rw_lock* lock)
{
	sem_init(&(lock->mutex),0,1);
  lock->reader_count = 0;
  lock->writer_count = 0;
  sem_init(&(lock->empty),0,1);
}

void writer_acquire(rw_lock* lock)
{
  sem_wait(&(lock->empty));
  //pthread_mutex_lock(&(lock->mutex));
  lock->writer_count++;
  //pthread_mutex_unlock(&(lock->mutex));
}

void writer_release(rw_lock* lock)
{
  //pthread_mutex_lock(&(lock->mutex));
  lock->writer_count--;
  sem_post(&(lock->empty));
  //pthread_mutex_unlock(&(lock->mutex));
}

void reader_acquire(rw_lock* lock)
{
  sem_wait(&(lock->mutex));
  if(lock->reader_count==0) {
    sem_wait(&(lock->empty));
  }
  lock->reader_count++;
  sem_post(&(lock->mutex));
}

void reader_release(rw_lock* lock)
{
  sem_wait(&(lock->mutex));
  lock->reader_count--;
  if(lock->reader_count==0) {
    sem_post(&(lock->empty));
  }
  sem_post(&(lock->mutex));
}

void cleanup(rw_lock* lock)
{
	sem_destroy(&(lock->mutex));
	sem_destroy(&(lock->empty));
}
