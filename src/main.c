#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include "../include/graph.h"
#include "../include/solution.h"
#include "../include/safe_citizens.h"
#include "../include/utils.h"

    void signal_handler(int signum) {
    if (signum == SIGTERM) {
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: %s <input_file> <num_processes> <timeout_ms>\n", argv[0]);
        return 1;
    }


// In main, before fork:
signal(SIGTERM, signal_handler);

    char* input_file = argv[1];
    int num_processes = atoi(argv[2]);
    int timeout_ms = atoi(argv[3]);

  if (num_processes <= 0 || timeout_ms <= 0) {
        fprintf(stderr, "Invalid number of processes or timeout value\n");
        return 1;
    }

    // Read input
    int M, N, num_citizens, num_supermarkets;
    Location *citizens, *supermarkets;
    read_input(input_file, &M, &N, &citizens, &num_citizens, &supermarkets, &num_supermarkets);

    // Setup shared memory
    int shmid = shmget(IPC_PRIVATE, sizeof(SharedMemory), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }
    SharedMemory* shared_mem = (SharedMemory*)shmat(shmid, NULL, 0);

    // Initialize semaphore
    sem_t* mutex = sem_open("/safe_mutex", O_CREAT, 0644, 1);
    shared_mem->mutex = mutex;
    shared_mem->best_solution.score = 0;
    shared_mem->iterations = 0;
    shared_mem->time_to_best = 0;


    pid_t* pids = malloc(num_processes * sizeof(pid_t));

    // Create processes
    for (int i = 0; i < num_processes; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {  // Child process
            srand(time(NULL) ^ getpid());
            
            Graph* g = create_graph(M, N);
            for (int j = 0; j < num_citizens; j++) {
                add_citizen(g, citizens[j]);
            }
            for (int j = 0; j < num_supermarkets; j++) {
                add_supermarket(g, supermarkets[j]);
            }

            run_fscs_process(g, shared_mem, num_citizens, num_supermarkets);
            
            destroy_graph(g);
            exit(0);
        }
    }

    // Wait for timeout
    usleep(timeout_ms * 1000);

    // Kill all child processes
    for (int i = 0; i < num_processes; i++) {
        kill(pids[i], SIGTERM);
    }

    // Print results
    char results_filename[256];
    sprintf(results_filename, "tests/file_results/res_instance_%d.txt", getpid());
    write_file_results(results_filename, timeout_ms, &shared_mem->best_solution,
                      shared_mem->iterations, shared_mem->time_to_best);

     // Cleanup
    sem_close(mutex);
    sem_unlink("/safe_mutex");
    shmdt(shared_mem);
    shmctl(shmid, IPC_RMID, NULL);
    free(citizens);
    free(supermarkets);
    free(pids);
    
    for (int i = 0; i < num_processes; i++) {
        wait(NULL);  // Wait for child processes to finish
    }

    return 0;
} 