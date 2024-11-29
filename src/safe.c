#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <time.h>
#include <fcntl.h>

#define MAX_VERTICES 1000
#define MAX_PATHS 100

typedef struct {
    int avenue;
    int street;
} Location;

typedef struct {
    int num_vertices;
    int *adjacency_list[MAX_VERTICES];
    int list_sizes[MAX_VERTICES];
} Graph;

typedef struct {
    int *path;
    int path_length;
} Path;

typedef struct {
    int num_paths;
    Path paths[MAX_PATHS];
    int score;
} Solution;

typedef struct {
    Solution best_solution;
    sem_t *mutex;
} SharedMemory;

// Function prototypes
Graph* create_graph(int M, int N);
void add_citizen(Graph *g, Location loc);
void add_supermarket(Graph *g, Location loc);
int find_safe_citizen(Graph *g, int *visited, Solution *current_solution);
Solution find_safe_citizens(Graph *g, int num_citizens, int num_supermarkets);
void read_input(char *filename, int *M, int *N, Location **citizens, int *num_citizens, 
                Location **supermarkets, int *num_supermarkets);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <input_file> <num_processes> <timeout_ms>\n", argv[0]);
        return 1;
    }

    char *input_file = argv[1];
    int num_processes = atoi(argv[2]);
    int timeout_ms = atoi(argv[3]);

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
    SharedMemory *shared_mem = (SharedMemory *)shmat(shmid, NULL, 0);

    // Initialize semaphore
    sem_t *mutex = sem_open("/safe_mutex", O_CREAT, 0644, 1);
    shared_mem->mutex = mutex;
    shared_mem->best_solution.score = 0;

    // Create processes
    for (int i = 0; i < num_processes; i++) {
        pid_t pid = fork();
        if (pid == 0) {  // Child process
            // Initialize random seed differently for each process
            srand(time(NULL) ^ getpid());
            
            Graph *g = create_graph(M, N);
            // Add citizens and supermarkets
            for (int j = 0; j < num_citizens; j++) {
                add_citizen(g, citizens[j]);
            }
            for (int j = 0; j < num_supermarkets; j++) {
                add_supermarket(g, supermarkets[j]);
            }

            // Run algorithm
            Solution solution = find_safe_citizens(g, num_citizens, num_supermarkets);

            // Update shared memory if better solution found
            sem_wait(mutex);
            if (solution.score > shared_mem->best_solution.score) {
                shared_mem->best_solution = solution;
            }
            sem_post(mutex);

            exit(0);
        }
    }

    // Wait for timeout
    usleep(timeout_ms * 1000);

    // Kill all child processes
    // Print results
    // Cleanup

    return 0;
} 