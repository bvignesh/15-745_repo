/* This file has shared variables that have different number of uses*/

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <map>
#include <pthread.h>
#include <stdio.h>
#include <ctime>
#include "gups.h"

#define NLOOPS 1000000  

int INPUT_SIZE;
int TABLE_SIZE;

int NTHREADS;

void * incr_function(void *);
void read_in();
double post_parallel_phase();

using namespace std;
int *data;

/*The approximation targets*/
double x1, x2, x3;    // The approximation targets

/*locks for each approximation target*/
pthread_mutex_t lock1, lock2, lock3;



int tids[NUM_THREADS];



int main(int argc, char** argv) {
    if (argc != 2) {
        printf("usage: ./uses <input size> <table size>\n");
        return -1;
    }
    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);
    pthread_mutex_init(&lock3, NULL);

    NTHREADS = atoi(argv[1]);  //This is the only argument to the program
    
    pthread_t threads[NUM_THREADS];
    int ret;
    
//    read_in();

    // Now we launch threads to go through the data and increment the counter accordingly.
    for (int i = 0; i < NUM_THREADS; i++) {
        tids[i] = i;
	if (i > 2)
        ret = pthread_create(&threads[i], NULL, incr_function1, (void*) &tids[i]);
	else
	ret = pthread_create(&threads[i], NULL, incr_function2, (void*) &tids[i]);
        if (ret) {
            fprintf(stderr, "Error - pthread_create() returned code - %d\n", ret);
            exit(EXIT_FAILURE);
        }
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    double p_output = post_parallel_phase();
     std::cout << "Program output is " << p_output/(double)NLOOPS << "\n";    
    return p_output;  //might be useful from liveness analysis perspective
}

/* The parallel portions of the program */
void * incr_function1(void * dummy) {
    int tid = *(int*)dummy;
    /*all the shared variables are being updated the same way*/
    for (int i = 0; i < NLOOPS; i++) {
        
	pthread_mutex_lock(&lock1);
        x1 += i*tid;  			//Update to shared variable that is different in different cores
        pthread_mutex_unlock(&lock1);
	pthread_mutex_lock(&lock2);
        x2 += i*tid;                    //Update to shared variable that is different in different cores
        pthread_mutex_unlock(&lock2);
	pthread_mutex_lock(&lock3);
        x3 += i*tid;                    //Update to shared variable that is different in different cores
        pthread_mutex_unlock(&lock3);
	
    }
}

void * incr_function2(void * dummy) {
    int tid = *(int*)dummy;
    /*all the shared variables are being updated the same way*/
    for (int i = 0; i < NLOOPS; i++) {
        
	pthread_mutex_lock(&lock1);
        x1 += i*tid; 
	x1 += tid;
	x1 += i; 			//Update to shared variable that is different in different cores
        pthread_mutex_unlock(&lock1);
	pthread_mutex_lock(&lock2);
        x2 += i*tid;
	// += i*i;                    //Update to shared variable that is different in different cores
        pthread_mutex_unlock(&lock2);
	pthread_mutex_lock(&lock3);
        x3 += i*tid;                    //Update to shared variable that is different in different cores
        pthread_mutex_unlock(&lock3);
	
    }
}


/*
void read_in() {
    // For now we read from the input file because it'll be easier to test things out.
    data = (int*)malloc(INPUT_SIZE * sizeof(int));
    srand(std::time(0));
    for (int i = 0; i < INPUT_SIZE; i++) {
        data[i] = rand() % TABLE_SIZE;
    }
}
*/

/* The serial portion that follows the parallel phase. Gives us uses for compiler passes and another 
   source of error in program*/
double post_parallel_phase() {
	double y1, y2, y3, final_out;
	/* Random computations that will fit a basic block */
	y1 = x1 + 17;
	y2 = x2 + 19;
	y3 = x3 + 23;
	
	y1 = x1*y1;
	y2 = x2*y2*y2;
	
	y1 = y1 + x1;
	
	y1 = y1/x2;
	y2 = y2/x1;
	y3 = y3/x1;

	final_out = x1 + x2 + x3;  
	return final_out;

	
}

