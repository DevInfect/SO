#include "concurrency.h"
#include "fscs.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void run_processes(int num_processes, Map *map, int num_people) {
    // initialize shared memory
    Path *paths = initialize_shared_memory(num_people);

    // initialize semaphore
    initialize_semaphore();

    // create the processes
    for (int i = 0; i < num_processes; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Error creating process");
            exit(1);
        } else if (pid == 0) {

            // child process
            lock_semaphore();
            int num_solutions = find_safe_citizen(map, paths, num_people);
            printf("Process %d found %d solutions\n", i, num_solutions);
            exit(0);
            unlock_semaphore();
            exit(0);

        }
    }

    // father process waits for the children
    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
    }

    // free resources
    free_resources(paths);
}