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

pthread_mutex_t* insertLock;
pthread_mutex_t* stopSearch;
sem_t* searchCount;


typedef struct ll ll;
struct ll{
    ll* next;
    int value;

};

typedef struct{
    ll* listhead;

}arg;

arg* argGen(){
    arg* tmp = (arg*)malloc(sizeof(arg) );
    tmp->listhead = (ll*)malloc(sizeof(ll) );
    tmp->listhead->next = NULL;
    tmp->listhead->value = -1;/*This is an implicit sentinel*/
    return tmp;
}


void *insert( void* args){
    arg* param = (arg*) args;
    ll* node;
    while(1){
        node = (ll*)malloc(sizeof(ll));
        node->value = 1;

        pthread_mutex_lock( insertLock );
        printf("INSERT: Adding to list, sleep 1\n");

        node->next = param->listhead->next;
        param->listhead->next = node;
        sleep(1);
        printf("INSERT: Releasing insertLock\n");
        pthread_mutex_unlock( insertLock );
        sleep(1);

    }
}

void *search( void* args){
    arg* param = (arg*) args;
    ll* curr = param->listhead;
    while(1){
        pthread_mutex_lock(stopSearch);
        pthread_mutex_unlock(stopSearch);
        sem_post(searchCount);
        printf("SEARCH: Searching list, sleep 1\n");
        sleep(1);
        while(curr){
            if( curr->value == 1){
                break;
            }else{
                curr = curr->next;
            }
        }
        sem_wait(searchCount);

    }
}

void *delete( void* args){
    arg* param = (arg*) args;
    int check;
    ll* curr, *tmp;
    while(1){
        pthread_mutex_lock(stopSearch);
        pthread_mutex_lock(insertLock);
        check = -1;
        while(check != 0){
            sem_getvalue(searchCount, &check); 
        }

        printf("DELETE: Deleting element from the list, sleep for 3\n");
        sleep(3);/*This is just to make it more obvious*/
        
        curr = param->listhead;
        while(curr->next){
            if( curr->next->value == 1 ){
                tmp = curr->next;
                curr->next = curr->next->next;
                free(tmp);
                break;
            }
            curr = curr->next;
        }

        printf("DELETE: Releaseing list to other threads\n");
        pthread_mutex_unlock(stopSearch);
        pthread_mutex_unlock(insertLock);
        sleep(1);
    }
}


void spawn(pthread_t* thr, int size, void* (*runThread)(void*), arg* args ){
    int i, check;
    for( i = 0; i < size; i++){
        arg* param = args;
        check = pthread_create(&thr[i], NULL, runThread, (void*) param);
        if (check!=0){
            fprintf(stderr, "Thread creation Failure\n");
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
    insertLock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) );
    stopSearch = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) );
    searchCount = (sem_t*)malloc(sizeof(sem_t));
    pthread_mutex_init(insertLock, NULL);
    pthread_mutex_init(stopSearch, NULL);
    sem_init(searchCount, 0, 0);
}

int main(int argc, char** argv){
    init();
    pthread_t threads[THREAD_COUNT];

    /*Spawn 3 inserters, 4 search, one delete*/
    arg* param = argGen();
    spawn(threads, 3, insert, param);
    spawn(threads+3, 2, search, param);
    spawn(threads+5, 1, delete, param);
    
    join(threads, 8);

    return 0;
}
