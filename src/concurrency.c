// concurrency.c
#include "concurrency.h"
#include "fscs.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#include <sys/time.h>

volatile int timeout = 0;

// Função para tratar o sinal de alarme
void timeout_handler(int sig) {
    timeout = 1;
}

// Função auxiliar para operar o semáforo
void operacao_semaforo(int semid, int valor) {
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = valor;
    op.sem_flg = 0;
    semop(semid, &op, 1);
}

int run_processes(int num_processes, const Mapa *mapa, int tempo_ms) {
    int num_safos = 0;

    signal(SIGALRM, timeout_handler);    

    struct itimerval timer;
    timer.it_value.tv_sec = tempo_ms / 1000;
    timer.it_value.tv_usec = (tempo_ms % 1000) * 1000;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);

    // Cria memória partilhada para armazenar o número total de cidadãos seguros
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("Erro ao criar memória partilhada");
        exit(1);
    }
    int *shared_num_safos = (int *)shmat(shmid, NULL, 0);
    *shared_num_safos = 0;

    // Configura o semáforo
    int semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("Erro ao criar semáforo");
        exit(1);
    }
    semctl(semid, 0, SETVAL, 1);  // Inicializa o semáforo com valor 1

    // Cria os processos
    for (int i = 0; i < num_processes; i++) {
        if (timeout) {
            printf("Tempo limite excedido. Encerrando execução.\n");
            break;
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("Erro ao criar processo");
            exit(1);
        } else if (pid == 0) {
            // Processo filho
            Caminho caminhos[mapa->num_cidadaos];
            int solucoes_locais = find_safe_citizen(mapa, caminhos);

            // Atualiza a contagem total de cidadãos seguros
            operacao_semaforo(semid, -1);  // Trava o semáforo
            *shared_num_safos += solucoes_locais;
            operacao_semaforo(semid, 1);   // Libera o semáforo

            exit(0);
        }
    }

    // Espera que todos os processos filhos terminem
    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
    }

    // Copia o valor final da memória partilhada para a variável local
    num_safos = *shared_num_safos;

    // Libera o semáforo e a memória partilhada
    shmdt(shared_num_safos);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return num_safos;
}
