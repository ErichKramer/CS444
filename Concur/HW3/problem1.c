/*
 * Erich Kramer Concurrency Homework 3 CS444
 * Problem 1, 
 *
 *
 * */
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define MAX 3

sem_t* busy;
sem_t* done;

pthread_mutex_t* accessLim;

//just make mutexes global smh
void *thread_exec(){
    int check;

    while(1){
        //get control of a sempahore value


        sem_wait(busy); /*Hang here if 3 running*/       



        printf("Sleep to simulate resource access\n");
        sleep(5);

        /*if the busy semaphore has a value = 0 then wait until all three are done */
        sem_getvalue( busy, &check);
        if( check == 0){
            sem_wait(done);
            sem_getvalue(done, &check);
            while(check!=0){
                sem_getvalue(done, &check);
            }
            sleep(1);
            printf("Release resource for other threads\n");
            sem_post(done);
        }
        
        //all of the threads realized we can move on, release resource
        sem_post(busy );

    }
}

void init( ){
    busy = (sem_t*)malloc(sizeof(sem_t));
    sem_init(busy,0, MAX);
    done = (sem_t*)malloc(sizeof(sem_t));
    sem_init(done,0, MAX);
    accessLim = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(accessLim, NULL);

}


int main(int argc, char** argv){
    init();


    pthread_t threads[MAX+2];
    for(int i =0; i< MAX+2; i++){
        printf("Spawn one thread\n");
        pthread_create( &threads[i], NULL, &thread_exec, NULL);

    }

    for(int i =0; i< MAX+2; i++){
        pthread_join(threads[i], NULL);

    }

    return 0;
}


