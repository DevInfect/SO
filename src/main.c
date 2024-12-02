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
#include <string.h>
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
    char* input_file;
    int num_processes, timeout_ms;

    if (argc == 2 && strcmp(argv[1], "--test") == 0) {
        // Parâmetros predefinidos para teste
        input_file = "tests/file_tests/instances_2.txt";
        num_processes = 4;
        timeout_ms = 5000;
        printf("Modo de teste ativado: Executando com arquivo '%s', %d processos, %d ms\n",
               input_file, num_processes, timeout_ms);
    } else if (argc == 4) {
        input_file = argv[1];
        num_processes = atoi(argv[2]);
        timeout_ms = atoi(argv[3]);

        if (num_processes <= 0 || timeout_ms <= 0) {
            fprintf(stderr, "Invalid number of processes or timeout value\n");
            return 1;
        }
    } else {
        printf("Usage: %s <input_file> <num_processes> <timeout_ms>\n", argv[0]);
        printf("       %s --test\n", argv[0]);
        return 1;
    }

    signal(SIGTERM, signal_handler);

    // Ler entrada
    int M, N, num_citizens, num_supermarkets;
    Location *citizens, *supermarkets;
    read_input(input_file, &M, &N, &citizens, &num_citizens, &supermarkets, &num_supermarkets);

    // Configurar memória partilhada
    int shmid = shmget(IPC_PRIVATE, sizeof(SharedMemory), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }
    SharedMemory* shared_mem = (SharedMemory*)shmat(shmid, NULL, 0);

    // Inicializar semáforo
    sem_t* mutex = sem_open("/safe_mutex", O_CREAT, 0644, 1);
    shared_mem->mutex = mutex;
    shared_mem->best_solution.score = 0;
    shared_mem->iterations = 0;
    shared_mem->time_to_best = 0;

    pid_t* pids = malloc(num_processes * sizeof(pid_t));

    // Capturar o tempo inicial
    long start_time = get_current_time_ms();

    // Criar processos
    for (int i = 0; i < num_processes; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {  // Processo filho
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

    // Esperar pelo timeout
    usleep(timeout_ms * 1000);

    // Terminar processos filhos
    for (int i = 0; i < num_processes; i++) {
        kill(pids[i], SIGTERM);
    }

    // Imprimir resultados
    print_solution(&shared_mem->best_solution, start_time, shared_mem->iterations, shared_mem->time_to_best);

    // Limpeza
    sem_close(mutex);
    sem_unlink("/safe_mutex");
    shmdt(shared_mem);
    shmctl(shmid, IPC_RMID, NULL);
    free(citizens);
    free(supermarkets);
    free(pids);

    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
    }

    return 0;
}