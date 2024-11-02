#ifndef CONCURRENCY_H
#define CONCURRENCY_H

#include "fscs.h"


// function to initialize the shared memory to store the paths of the citizens
Path *initialize_shared_memory(int num_people);

// function to initialize the semaphore
void initialize_semaphore();

// function to lock the semaphore
void lock_semaphore();

// function to unlock the semaphore
void unlock_semaphore();

// function to free the resources
void free_resources(Path *paths);

#endif