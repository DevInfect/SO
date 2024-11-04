#include "input.h"
#include "fscs.h"
#include "concurrency.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <ficheiro_input> <num_processos> <tempo_ms>\n", argv[0]);
        return 1;
    }

    // Processa os argumentos de entrada
    char *ficheiro_nome = argv[1];
    int num_processes = atoi(argv[2]);
    int tempo_ms = atoi(argv[3]);

    if (num_processes <= 0 || tempo_ms <= 0) {
        fprintf(stderr, "Erro: Número de processos e tempo devem ser maiores que zero.\n");
        return 1;
    }

    // Carrega o input do ficheiro
    Mapa mapa;
    if (load_input(ficheiro_nome, &mapa) != 0) {
        fprintf(stderr, "Erro ao carregar o ficheiro de input.\n");
        return 1;
    }

    // Executa o algoritmo com processos concorrentes
    int num_cidadaos_safos = run_processes(num_processes, &mapa, tempo_ms);

    // Exibe o resultado final
    printf("Número máximo de cidadãos que podem chegar ao supermercado: %d\n", num_cidadaos_safos);

    // Libera a memória alocada para o mapa
    free(mapa.supermercados);
    free(mapa.cidadaos);

    return 0;
}
