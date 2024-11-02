#include "concurrency.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

//global indicator for the shared memory segment
int shmid;
int semid;

// function to initialize the shared memory
Path *initialize_shared_memory(int num_people) {
    // allocate shared memory to store the paths of each citizen
    shmid = shmget(IPC_PRIVATE, num_people * sizeof(Path), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("Error allocating shared memory");
        exit(1);
    }

    // associate the shared memory segment to the pointer
    Path *paths = (Path*) shmat(shmid, NULL, 0);
    if(paths == (Path*) -1) {
        perror("Error associating shared memory");
        exit(1);
    }

    return paths;

}

void initialize_semaphore() {
    // create a semaphore
    semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (semid < 0) {
        perror("Error creating semaphore");
        exit(1);
    }

    // initialize the semaphore with value 1 (unlocked)
    semctl(semid, 0, SETVAL, 1);
}

// lock the semaphore
void lock_semaphore() {
    struct sembuf sem_lock = {0, -1, 0};
    semop(semid, &sem_lock, 1);
}

// unlock the semaphore
void unlock_semaphore() {
    struct sembuf sem_unlock = {0, 1, 0};
    semop(semid, &sem_unlock, 1);
}

// free the resources
void free_resources(Path *paths) {
    // detach the shared memory segment
    shmdt(paths);

    // remove the shared memory segment
    shmctl(shmid, IPC_RMID, NULL);

    // remove the semaphore
    semctl(semid, 0, IPC_RMID);
}