//
//  Test.cpp
//  
//
//  Created by Dhruva on 3/19/15.
//
//

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <map>
#include <pthread.h>
#include <stdio.h>
#include <ctime>

#include "gups.h"
/*
    Input has 8096 ints in the range [0, 1024). Each thread goes through a chunk of this
    input and increments the count of that value.
*/
int INPUT_SIZE;
int TABLE_SIZE;

void * incr_function(void *);
void read_in();
void write_out();

using namespace std;
int *data;
int * hash_table;
pthread_mutex_t * mutex_table;

void register_tids(int * tids, int nindices, int nlocks) {
}

void* cmalloc(size_t s, int index) {
    return malloc(s);
}

void start_region(int tid, int loc) {
  pthread_mutex_lock(&mutex_table[loc]);
}

void end_region(int tid, int loc) {
  pthread_mutex_unlock(&mutex_table[loc]);
}

void do_merge(int tid) {}


int tids[NUM_THREADS];

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("usage: ./gups <input size> <table size>\n");
        return -1;
    }
    INPUT_SIZE = atoi(argv[1]);
    TABLE_SIZE = atoi(argv[2]);

    pthread_t threads[NUM_THREADS];
    int ret;
    
    read_in();
    register_tids(tids, 1, TABLE_SIZE);

    hash_table = (int *) cmalloc(sizeof(int) * TABLE_SIZE, 0);
    mutex_table = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t) * TABLE_SIZE);

    for (int i = 0; i < TABLE_SIZE; i++) {
      hash_table[i] = 0;
      pthread_mutex_init(&mutex_table[i], NULL);
    }
    
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

    write_out();
   return 0;
}

void * incr_function(void * dummy) {
    int tid = *(int*)dummy;
    int start = (INPUT_SIZE/NUM_THREADS) * tid;
    int end = min((INPUT_SIZE/NUM_THREADS) * (tid + 1), INPUT_SIZE);
    for (int i = start; i < end; i++) {
        int loc = data[i];
        start_region(tid, loc);
	hash_table[loc]++;
        end_region(tid, loc);
    }
    do_merge(tid);
}


void read_in() {
    // For now we read from the input file because it'll be easier to test things out.
    data = (int*)malloc(INPUT_SIZE * sizeof(int));
    srand(std::time(0));
    for (int i = 0; i < INPUT_SIZE; i++) {
        data[i] = rand() % TABLE_SIZE;
    }
}

void write_out() {
    int error = 0;
    int err_0 = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hash_table[i] == 0)
            err_0++;
    }
    for (int i = 0; i < INPUT_SIZE; i++) {
        hash_table[data[i]]--;
        if (data[i] > TABLE_SIZE)
            error++;
        if (data[i] < 0)
            error++;
    }
    if (error != 0)
        cout << "ERROR_GEN: " << error << "\n";
    error = 0;
    int err_g = 0;
    int err_l = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hash_table[i] != 0)
            error++;
        if (hash_table[i] < 0)
            err_l++;
        if (hash_table[i] > 0)
            err_g++;
    }
    if (error != 0)
        cout << "ERROR: " << error << " " << err_l << " " << err_g << " " << err_0 << "\n";
}

