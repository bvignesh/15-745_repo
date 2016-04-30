/* File implements the case where we use the partial vs full redundancy heuristic */

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <map>
#include <pthread.h>
#include <stdio.h>
#include <ctime>
#include "gups.h"
#include <time.h>
#include <stdlib.h>


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
double x1, x2, x3_c;    // The approximation targets

double x3[NTHREADS];   //TODO: Junhan declare more variables as arrays for greater approximation

/*locks for each approximation target*/
pthread_mutex_t lock1, lock2, lock3;



int tids[NUM_THREADS];



int main(int argc, char** argv) {
    if (argc != 3) {
        printf("usage: ./uses <input size> <table size>\n");
        return -1;
    }
    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);
    pthread_mutex_init(&lock3, NULL);

    NTHREADS = atoi(argv[1]);  //This is the only argument to the program
    
    pthread_t threads[NUM_THREADS];
    int ret;
    
    //read_in();

    // Now we launch threads to go through the data and increment the counter accordingly.
    for (int i = 0; i < NUM_THREADS; i++) {
        tids[i] = i;
        ret = pthread_create(&threads[i], NULL, incr_function, (void*) &tids[i]);
        if (ret) {
            fprintf(stderr, "Error - pthread_create() returned code - %d\n", ret);
            exit(EXIT_FAILURE);
        }
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    double p_output = post_parallel_phase();
    
    return p_output;  //might be useful from liveness analysis perspective
}

/* The parallel portion of the program */
void * incr_function(void * dummy) {
    int tid = *(int*)dummy;
    /*all the shared variables are being updated the same way*/
    for (int i = 0; i < NLOOPS; i++) {
        pthread_mutex_lock(&lock1);
        x1 += tid*i;  			//Update to shared variable that is different in different cores
        pthread_mutex_unlock(&lock1);
	pthread_mutex_lock(&lock2);
        x2 += tid*i;                    //Update to shared variable that is different in different cores
        pthread_mutex_unlock(&lock2);
	pthread_mutex_lock(&lock3);
        x3[tid] += tid*i;                    //Update to shared variable that is different in different cores
        pthread_mutex_unlock(&lock3);
	
    }
    pthread_mutex_lock(&lock1);
    int temp;
    double merge_value1 = 0.0;
    double merge_value2 = 0.0;
    double merge_value3 = 0.0;
    int x = 0;
    while (x < MERGE_NO) {
    srand(x);
    temp = rand()%NUM_THREADS;
    merge_value1 += x3[temp];
    //srand(x + 1);
    //temp = rand()%NUM_THREADS;
    //merge_value2 += x2[temp];
    //srand(x + 2);
    //temp = rand()%NUM_THREADS;
    //merge_value3 += x1[temp];

    x++;
    }
    x3_c = (merge_value1/MERGE_NO)*(NUM_THREADS - MERGE_NO);  // this is the approximation
    x3_c = merge_value1 + x3_c;
    //x2_c = (merge_value2/MERGE_NO)*(NUM_THREADS - MERGE_NO);  // this is the approximation
    //x2_c = merge_value2 + x2_c;
    //x3_c = (merge_value3/MERGE_NO)*(NUM_THREADS - MERGE_NO);  // this is the approximation
    //x3_c = merge_value3 + x1_c;
    pthread_mutex_unlock(&lock1);

 	
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
	srand(time(NULL));
	if (rand() % 100 <= 10)
		y1 = x1 + 17;
	else
		y1 = x1 + 19;
	srand(time(NULL));
	if (rand() % 100 < 15) {
		y2 = x2 + 11;
		y1 = y1 + x1;
	}
	else {
		y1 = x1 + 15;
	}
	srand(time(NULL));
	if (rand() % 100 > 90) {
                y3 = x3_c + 11;
                y1 = y1 + x1;
        }
        else {
                y3 = x3_c + 15;
        }
	srand(time(NULL));
	if (rand() % 100 > 95) {
                y2 = x2 - y2;
                y1 = y1 * x1;
        }
        else { 
                y3 = x3_c + 15;
		y2 = x2 * y2;
		y1 = y1 - x1;
        }

	//I was thinking about this and I think we need more cases wherein we have partial redundancy
	//and call them approximable


	

	final_out = y1 + y2 + y3;  
	return final_out;

	
}

