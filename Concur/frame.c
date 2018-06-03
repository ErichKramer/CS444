/*
 * Erich Kramer Concurrency Homework 3 CS444
 * Problem 2, 
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
 
    /*Important population goes here*/

    return tmp;   
}


void *thread_exec( void* args){
    printf("thread execution began\n");

    return NULL;
}


void spawn(pthread_t* thr, int size, void* (*runThread)(void*), arg* args ){
    int i, check;
    for( i = 0; i < size; i++){
        check = pthread_create(&thr[i], NULL, runThread, (void*) args);
        if( check != 0){
            fprintf(stderr, "Thread Creation Failure\n");
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


void init(){

}

int main(int argc, char** argv){
    init();
    pthread_t threads[THREAD_COUNT];
    
    arg* param = argGen();
    spawn(threads, 1, thread_exec, param);
    join(threads, 1);

    return 0;
}
