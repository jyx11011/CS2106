/*************************************
 * Lab 3 Exercise 4
 * Name: Jiang Yuxin
 * Student No: A0177868A
 * Lab Group: 11
 *************************************/
 
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include<stdio.h>
#include<errno.h>

#include "traffic_synchronizer.h"

//Using extern, you can use the global variables num_of_cars and num_of_segments from ex3_runner.c in your code.
extern int num_of_cars;
extern int num_of_segments;

pthread_mutex_t* seg_mutex;
pthread_mutex_t* ent_mutex;
sem_t emptySeg;

void initialise()
{
    int i;
	seg_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t)*num_of_segments);
    ent_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t)*num_of_segments);
	for(i=0;i<num_of_segments;i++){
		pthread_mutex_init(&seg_mutex[i],NULL);
        pthread_mutex_init(&ent_mutex[i],NULL);
	}
    sem_init(&emptySeg,0,num_of_segments-1);
}

void cleanup()
{
	int i;
	for(i=0;i<num_of_segments;i++){
		pthread_mutex_destroy(&seg_mutex[i]);
        pthread_mutex_destroy(&ent_mutex[i]); 
    }
	free(seg_mutex);
    free(ent_mutex);
}

void* car(void* car)
{
	car_struct *c = (car_struct*)car;
    //This function modeles a thread 
    //A car: 
    //   -should call enter_roundabout (...)
    //   -followed by some calls to move_to_next_segment (...)
    //   -finally call exit_roundabout (...)
    sem_wait(&emptySeg);
    pthread_mutex_lock(&(ent_mutex[c->entry_seg]));
    pthread_mutex_lock(&(seg_mutex[c->entry_seg]));
    int next = NEXT(c->entry_seg, num_of_segments);
    pthread_mutex_lock(&(ent_mutex[next]));
    enter_roundabout(c);
    pthread_mutex_unlock(&(ent_mutex[c->entry_seg]));
    while(c->current_seg!=c->exit_seg) {
        int prev = c->current_seg;
        int next = NEXT(c->current_seg, num_of_segments);
    	pthread_mutex_lock(&(seg_mutex[next]));
        if(next!=c->exit_seg) 
            pthread_mutex_lock(&(ent_mutex[NEXT(next,num_of_segments)]));
        move_to_next_segment(c);
        pthread_mutex_unlock(&(ent_mutex[next]));
    	pthread_mutex_unlock(&(seg_mutex[prev]));
    }
    exit_roundabout(c);
    
    pthread_mutex_unlock(&(seg_mutex[c->exit_seg]));
    sem_post(&emptySeg);
    pthread_exit(NULL);
}
