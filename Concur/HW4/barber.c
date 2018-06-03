/*
 * Erich Kramer Concurrency Homework 4 CS444
 * Barbershop problem
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

#define THREAD_COUNT 10


pthread_mutex_t* barberChair;
sem_t* customerChairs;

typedef struct{
    


}arg;


arg* argGen(){
    arg* tmp = (arg*)malloc(sizeof(arg) );
    
    return tmp;
}

void *barber( void* args){

}

void *customer( void* args){

}




void spawn(pthread_t* thr, int size, void* (*runThread)(void*), arg* args ){
    int i, check;
    for( i = 0; i < size; i++){
        arg* param = buildArg();
        check = pthread_create(&thr[i], NULL, runThread, (void*) param); 
        if( check != 0){
            fprintf( stderr, "Thread Creation Failure\n");
            exit(1);
        }
    }
}

void join(pthread_t* thr, int size ){
    int i;
    for( i=0; i< size; i++){
        pthread_join(thr[i], NULL);
    }

}

void init(int seats){

}

int main(int argc, char** argv){
    int seats = 10;

    pthread_t threads[THREAD_COUNT];

    arg* param = argGen();
    spawn(threads, 1, barber, param);
    spawn(threads, THREAD_COUNT-1, customer, param);

    join(threads, 10);

    return 0;
}
