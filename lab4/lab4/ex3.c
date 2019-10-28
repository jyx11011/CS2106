/*************************************
 * Lab 4 Exercise 3
 * Name: Jiang Yuxin
 * Student No: A0177868A
 * Lab Group: 11
 *************************************/

// You can modify anything in this file. 
// Unless otherwise stated, a line of code being present in this template 
//  does not imply that it is correct/necessary! 
// You can also add any global or local variables you need (e.g. to implement your page replacement algorithm).

#include <signal.h>
#include <stdlib.h>
#include<stdio.h>

#include "api.h"

void os_run(int initial_num_pages, page_table *pg_table){
    // The main loop of your memory manager
    sigset_t signals;
    sigemptyset(&signals);
    sigaddset(&signals, SIGUSR1);
    sigaddset(&signals, SIGUSR2);
    
    // create the pages in the disk first, because every page must be backed by the disk for this lab
    for (int i=0; i!=initial_num_pages; ++i) {
        disk_create(i);
    }
    
    int frame_page[1<<FRAME_BITS];
    int frame_size=1<<FRAME_BITS;

    int* page_mapped = (int*)(malloc(sizeof(int)*initial_num_pages));
    int arr_size = initial_num_pages;
    int num_pages = initial_num_pages;

    for(int i=0;i<initial_num_pages;i++){
        page_mapped[i]=1;
    }
    
    for(int i=0;i<frame_size;i++)
        frame_page[i]=-1;
    int next_victim = 0;

    while (1) {
        siginfo_t info;
        int signum=sigwaitinfo(&signals, &info);
        
        if(signum==SIGUSR1){
        // retrieve the index of the page that the user program wants, or -1 if the user program has terminated
            int const requested_page = info.si_value.sival_int;
            if (requested_page == -1) break;
            if (!(requested_page < num_pages && page_mapped[requested_page]==1)) {
                union sigval reply_value;
                reply_value.sival_int = 1;
                sigqueue(info.si_pid, SIGCONT, reply_value);
            } else {
                // process the signal, and update the page table as necessary
                while(frame_page[next_victim]!=-1 
                    && pg_table->entries[frame_page[next_victim]].referenced != 0){
                    pg_table->entries[frame_page[next_victim]].referenced = 0;
                    next_victim = (next_victim+1)%frame_size;
                }
                if(frame_page[next_victim]!=-1){
                    if (pg_table->entries[frame_page[next_victim]].dirty ==1) {
                        disk_write(next_victim, frame_page[next_victim]);
                        pg_table->entries[frame_page[next_victim]].dirty=0;
                    }
                    pg_table->entries[frame_page[next_victim]].valid = 0;
                }
                frame_page[next_victim]=requested_page;
                disk_read(next_victim, requested_page);
                pg_table->entries[requested_page].valid = 1;
                pg_table->entries[requested_page].frame_index=next_victim;
                pg_table->entries[requested_page].referenced=0;
                next_victim=(next_victim+1)%frame_size;

                // tell the MMU that we are done updating the page table
                union sigval reply_value;
                reply_value.sival_int = 0; // set to 0 if the page is successfully loaded, set to 1 if the page is not mapped to the user process (i.e. segfault)
                sigqueue(info.si_pid, SIGCONT, reply_value);
            }
        } else {
            int const page_num = info.si_value.sival_int;
            if(page_num==-1){
                union sigval reply_value;
                reply_value.sival_int = num_pages++;
                if(num_pages>arr_size){
                    page_mapped = (int*)realloc(page_mapped, sizeof(int)*arr_size*2);
                    for(int i=arr_size;i<arr_size*2;i++) page_mapped[i]=0;
                    arr_size*=2;
                }
                page_mapped[num_pages-1]=1;
                disk_create(num_pages-1);
                sigqueue(info.si_pid, SIGCONT, reply_value);
            } else {
               
                if (page_num < num_pages && page_mapped[page_num]==1){ 
                    page_mapped[page_num]=0;
                    if(pg_table->entries[page_num].valid==1){
                        pg_table->entries[page_num].dirty=0;
                        pg_table->entries[page_num].valid=0;
                    }
                    disk_delete(page_num);
                }
                union sigval reply_value;
                reply_value.sival_int = 0;
                sigqueue(info.si_pid, SIGCONT, reply_value);
            }

        }
    }
    free(page_mapped);
    
}
    