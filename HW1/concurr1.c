#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include "mt19937ar.h"

#define BUFF_SIZE 32

/*I know globals are bad but c'mon*/
int rdrand = 0;

/*objects to be both consumed and produced*/
typedef struct{
    int output;
    int waittime;//between 2 and 9

} obj;

/*used to pass arguments to consumer and producer*/
typedef struct{
    int count;
    int buffCurr;
    obj** buff;
    pthread_mutex_t* buffAccess;
    sem_t* buffFull;
    sem_t* buffEmpty;
     
}arg;

/*Specified argument construction for consumer/producer threads*/
arg* buildArg(int c){
    arg* tmp = malloc( sizeof(arg));
    tmp->count = c;
    /*Init buffer of object pointers*/
    tmp->buff = malloc(sizeof(obj*) * BUFF_SIZE ); 
    tmp->buffCurr = 0;
    /*Create and init mutex*/
    tmp->buffAccess = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(tmp->buffAccess, NULL);
    /*Create and init semaphore for empty and for full buffer*/
    tmp->buffFull = (sem_t*)malloc(sizeof(sem_t));
    sem_init(tmp->buffFull, 0, BUFF_SIZE);/*Init max, producer will wait on add*/
    tmp->buffEmpty = (sem_t*)malloc(sizeof(sem_t));
    sem_init(tmp->buffEmpty, 0, 0); /*Init at 0, consumer waits and blocks*/

    return tmp;
}

/*Becasue arg objects have allocated things and 
 * semaphore objects we clean them up formally*/
void cleanupArg(arg* deathrow){
    int check = 0;
    check |= pthread_mutex_destroy(deathrow->buffAccess);
    check |= sem_destroy(deathrow->buffFull);
    check |= sem_destroy(deathrow->buffEmpty);
    if(check !=0){/*Super informative error message*/
        fprintf(stderr, "Shits fucked yo.\n");
        exit(1);
    }
    free(deathrow->buffAccess);
    free(deathrow->buffFull);
    free(deathrow->buffEmpty);
    free(deathrow->buff);
    free(deathrow);
}


/*Use code from mccgrath and kramerer to init random and set global var rdrand*/
void initRand(){

	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;
	eax = 0x01;
	__asm__ __volatile__(
	                     "cpuid;"
	                     : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
	                     : "a"(eax)
	                     );
	
	if( (ecx & 0x40000000) ){
	    printf("Using RdRand RNG\n");
        //use rdrand
        rdrand = 1; 
	}
	else{
        printf("Using mersenne RNG\n");
		//use mt19937, seed it because why not do it here
        init_genrand(time(NULL));
	}
    return;
}


int getRand(int lbound, int span){
    /*lbound is lowest number, lbound+span is highest number*/
    unsigned int rand;
    if(rdrand){
        __asm__ volatile(
                "rdrand %0;" : "=r" (rand)
                );
        //use rdrand asm
    }
    else{
        rand = genrand_int32();
    }
    return rand%span + lbound;
}

/*Randomly generate a buffer object for us to juggle*/
obj* buildObj(){
    obj* tmp = malloc( sizeof(obj));
    tmp->output = getRand(0, 10000);
    tmp->waittime = getRand(2,7);/*from 2 to 9*/
    return tmp;
}

void destroyObj(obj* tmp){
    free(tmp);
    return;
}

/*Wait on buffer full, add something to it after waiting and */
void *producer(void* args){

    //printf("Inside producer\n");
    arg* input = args;
    int count = input->count;
    obj* tmp;
    unsigned int sTime;
    /*run infinitely if count is 0, run finite if number is specified*/
    while( (--count)+1 || count<0 ){
        
        sTime = getRand(2,5);
        printf("producer sleeping: %u\n", sTime);
        sleep( sTime);

        /*Wait here if buffer is full*/
        
        tmp = buildObj();
        sem_wait(input->buffFull);

        /*SEM GETVALUE IS NOT TO BE TRUSTED*/
        
        /*Try to grab control of the buffer IO*/
        pthread_mutex_lock(input->buffAccess);
        
        /*If we are here we know for certain we have space in the buffer*/
        

        printf("Producing %u\n", tmp->output);
        input->buff[input->buffCurr++ ] = tmp;
        

        /*Inform world that one more element exists in buffer*/
        sem_post(input->buffEmpty);
        pthread_mutex_unlock(input->buffAccess);

    }
    return NULL;
}

/*Wait on buffer empty, until non 0, then consume -- decrement buff empty
 * After consumption post to buffer full, as this is inverse of empty sem*/
void *consumer(void* args){

    //printf("Inside consumer\n");
    arg* input = args;
    int count = input->count;
    obj* tmp;
    while((--count)+1 || count<0){
        
        /*Wait here if buffer is empty*/
        sem_wait(input->buffEmpty);

        /*Try to grab control of the buffer IO*/
        
        pthread_mutex_lock(input->buffAccess);

        /*A dissapointing hack that works. Sem_getvalue is filled with spiders*/
        tmp = input->buff[--input->buffCurr];
        input->buff[input->buffCurr] = NULL;
        
        /*Updatet the fact that buffer has lost a value*/
        sem_post(input->buffFull);
        printf("Buffer Size: %d\n",input->buffCurr );
        
        pthread_mutex_unlock(input->buffAccess);

        
        printf("consuming %u\n", tmp->output);
        printf("Consumer Sleep: %u\n", tmp->waittime);
        sleep( tmp->waittime);
        destroyObj(tmp);
        
    }

    return NULL;
}



int main(int argc, char **argv)
{
    int check;
    initRand();
    
    int iters = 0;
    if( argc >= 4){
        iters = atoi(argv[3]);
        printf("%d\n",iters);
    }
    int producers, consumers;
    if(argc >=3){
        producers = atoi(argv[1]);   
        consumers = atoi(argv[2]);
        printf("Chosen Producer Quantity: %d\n", producers);
        printf("Chosen Consumerer Quantity: %d\n", consumers);

    }
    else{
        fprintf(stderr, "USAGE: consumerproducer [PROD_THREADS] [CONSUMER_THRE"
                "ADS] [optional iteration count]\n");
        exit(10);
    }

    arg* prodArgs = buildArg(iters);
    arg* consArgs = prodArgs;   /*Here we are using the same arguments*/
                                /*Otherwise we should implement a clone ARgs function*/
    
    int i;
    for(i = 0; i< producers; i++){
        check = pthread_create( &(threadID[0]), NULL, &producer, (void*)prodArgs);
        if (check !=0){
            fprintf(stderr, "Producer thread creation failed. Unexpected state. Exiting\n");
            exit(1); 
        }
    }

    for(i = 0; i < consumers; i++){
        check = pthread_create( &(threadID[0]), NULL, &consumer, (void*)consArgs);
        if (check !=0){
            fprintf(stderr, "consumer thread creation failed. unexpected state. exiting\n");
            exit(1); 
        }
    }

    /*End main execution by tying up the threads*/
    pthread_join(threadID[0], NULL);
    pthread_join(threadID[1], NULL);

    cleanupArg(prodArgs);

	return 0;
}



