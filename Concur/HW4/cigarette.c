/*
 * Erich Kramer Concurrency Homework 3 CS444
 * Problem 2, 
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

typedef struct{

}arg;


arg* argGen(){
    arg* tmp = (arg*)malloc(sizeof(arg) );
    
}


void *thread_exec( void* args){

}


void spawn(pthread_t* thr, int size, runThread, buildArg ){
    int i, check;
    for( i = 0; i < size; i++){
        arg* param = buildArg();
        check = pthread_create(&thr[i], NULL, &
    }
}

void join(pthread_t* thr, int size ){
    int i;
    for( i=0; i< size; i++){
        pthread_join(thr[i], NULL);
    }

}

int main(int argc, char** argv){

    pthread_t threads[THREAD_COUNT];

    


    return 0;
}
