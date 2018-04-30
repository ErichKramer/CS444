#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "mt19937ar.h"

#define P 5
//number of philosophers

pthread_mutex_t* table;

typedef struct{
    pthread_mutex_t* left;
    pthread_mutex_t* right;
    char* name;

}arg;

arg* buildArg(char* name, pthread_mutex_t* l, pthread_mutex_t* r){
    arg* tmp = (char*)malloc( sizeof(arg) );
    tmp->name = strdup( name );
    tmp->pthread_mutex_t* left = l;
    tmp->pthread_mutex_t* right = r;
    return buildARg;
}


/*first lock the table so no one else can grab
 *try to pick up two forks, if we can then we proceed to eat
 *if we only get one fork then we put them down and wait paitently 
 *for our chance at the table again.*/

void getFork(){


}

/*just wait for 2-9 seconds*/
void eat(arg* args ){

}

/*Think about how hungry you are for 1-20 seconds*/
void think(arg* args){


}

void* philosopher(void* args){
    arg* input = (arg*) args;
}


int main(int argc, char** argv){

    /*using mersenne because It was easier than looking up the builtins
     * since I still have my reference code and a google search takes a while
     * Probably typing this comment takes longer though so... */
    pthread_t philos[P];
    pthread_mutex_t* forks[P];

    const char* names = { "McGrath", "McGrathBotA", "McGrathBotB", 
            "McGrathBotC", "McGrathBotD"};

    //how can I make this better, three loops in a row seems bad
    int i;
    for( i = 0; i < P; i++){
        pthread_mutex_init(forks[i]);
    }

    for( i = 0; i < P ; i++){
        arg* param = buildArg( names[i], forks

    }

    for( i = 0; i< P; i++){
        
    }


}



