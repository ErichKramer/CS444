#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "mt19937ar.h"

#define P 5
//number of philosophers

sem_t* table;

/*Build a struct for arguments to philosophers*/
typedef struct{
    pthread_mutex_t* left;
    pthread_mutex_t* right;
    char* name;

}arg;

/*Provide a means to init that same struct above in a clean manner*/
arg* buildArg(char* name, pthread_mutex_t* l, pthread_mutex_t* r){
    arg* tmp = (arg*)malloc( sizeof(arg) );
    tmp->name = strdup( name );
    tmp->left = l;
    tmp->right = r;
    return tmp;
}
/*If it was part of the assignment spec this is where I would make a destructor*/


//my own random function
int myRand(int lbound, int span){
    return genrand_int32()%span + lbound;
}


/*first lock the table so no one else can grab
 *try to pick up two forks, if we can then we proceed to eat
 *if we only get one fork then we put them down and wait paitently 
 *for our chance at the table again.*/

void getFork(arg* args){
    int check;

    while(1){
        sem_wait(table);    /*Recall table is our global*/
        check = pthread_mutex_trylock(args->left);
        if (check != 0){
            sem_post(table);
            continue;
        }


        check = pthread_mutex_trylock(args->right);
        if (check != 0){
            sem_post(table);
            pthread_mutex_unlock(args->left);
            continue;
        }
        /*Remember to unlock left if we can't also lock right. We
         * just jump back in line and wait for a fork to free up    */

        sem_post(table);  /*We got ours, now take it and run*/
        break;
    }
    /* try lock one,
     * if not lock break, release table
     * try lock other
     * if not lock break, release table*/

}

/*just wait for 2-9 seconds*/
void eat(arg* args ){
    printf("OM NOM NOM, %s is one hungry boi!\n", args->name);
    sleep(myRand(2, 9) );
    printf("%s finished eating!\n", args->name);
}

void putFork(arg* args){
    printf("%s is placing their forks down.\n", args->name);
    
    pthread_mutex_unlock(args->left);
    pthread_mutex_unlock(args->right);

}

/*Think about how hungry you are for 1-20 seconds*/
void think(arg* args){
    printf("%s is thinking about ways to make students cry.\n", 
            args->name);
    sleep(myRand(1, 20));
    printf("%s is really tired from thinking. Time to eat!\n", args->name);
    return;
}

void* philosopher(void* args){
    arg* philo = (arg*) args;
    printf("Spinning up bot: %s\n", philo->name);
    while(1){
        think(philo);
        getFork(philo);
        eat(philo);
        putFork(philo);
    }
    /*Can't leak memory if you never stop using it*/
    return NULL;
}


int main(int argc, char** argv){

    /*using mersenne because It was easier than looking up the builtins
     * since I still have my reference code and a google search takes a while
     * Probably typing this comment takes longer though so... */
    init_genrand(time(NULL));
    pthread_t philos[P];
    pthread_mutex_t* forks[P];

    char* names[5] = { "McGrath", "McGrathBotA", "McGrathBotB", 
            "McGrathBotC", "McGrathBotD"};


    table = (sem_t*)malloc(sizeof(sem_t));
    sem_init(table, 0, 1);

    int i, check;
    for( i = 0; i < P; i++){
        forks[i] = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(forks[i], NULL);
    }
    

    for( i = 0; i < P ; i++){
        arg* param = buildArg( names[i], forks[(i+P-1)%P], forks[(i+1)%P]);
        printf("%s is near forks %d and %d\n", names[i], i, (i+1)%P);
        check = pthread_create(&philos[i], NULL, &philosopher, (void*)param);
        if (check != 0){
            fprintf(stderr, "Thread spawn failed. Unexpected, exiting.\n");
            exit(1);
        }
    }

    for( i = 0; i< P; i++){
        pthread_join(philos[i], NULL);
    }
    return 0;
}



