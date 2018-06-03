/*
 * Erich Kramer Concurrency Homework 4 CS444
 * Barbershop problem
 *
 * */
#define _GNU_SOURCE
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define THREAD_COUNT 10


pthread_mutex_t* barberChair;
sem_t* customerChairs;

typedef struct{
    pthread_mutex_t* bChair;
    sem_t* cChairs;
}arg;


arg* argGen(){
    arg* tmp = (arg*)malloc(sizeof(arg) );

    tmp->bChair = barberChair;
    tmp->cChairs = customerChairs;

    return tmp;
}

void *barber( void* args){
    //arg* barbP = (arg*) args;
    printf("The barber blinks into existence!\n");

    return NULL;
}

void *customer( void* args){
    //arg* custP = (arg*) args;
    printf("Customer has entered the store!\n");
    pthread_mutex_lock( barberChair);

    return NULL;
}




void spawn(pthread_t* thr, int size, void* (*runThread)(void*), arg* args ){
    int i, check;
    for( i = 0; i < size; i++){
        check = pthread_create(&thr[i], NULL, runThread, (void*) args); 
        if( check != 0){
            fprintf( stderr, "Thread Creation Failure\n");
            exit(1);
        }
    }
    return;
}

void join(pthread_t* thr, int size ){
    int i;
    for( i=0; i< size; i++){
        pthread_join(thr[i], NULL);
    }

    return;
}

void init(int seats){
    barberChair = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    customerChairs = (sem_t*)malloc(sizeof(sem_t));

    pthread_mutex_init(barberChair, NULL);
    sem_init(customerChairs, 0, seats);
    return;
}

int main(int argc, char** argv){
    int i = 0;
    int joinStatus= 0;
    int seats = THREAD_COUNT-3;
    init(seats);

    pthread_t threads[THREAD_COUNT];/*Extra customers that leave immedietly*/

    /*Basic inits and my boring arg param*/
    arg* param = argGen();
    spawn(threads, 1, barber, param);
    spawn(threads+1, THREAD_COUNT-1, customer, param);

    /*No I want 10 thread running at all times, damnit!*/
    while(1){
        /*Get index in range (1,THREADCOUNT)*/
        i =  i%(THREAD_COUNT-1);
        i = i+1;
        /*Attempt a join*/
        joinStatus = pthread_tryjoin_np( threads[i], NULL);
        if(!joinStatus){
            /*We joined the thread*/
            spawn(threads+i, 1, customer, param);
        }

    }
    

    join(threads, 10);

    return 0;
}
