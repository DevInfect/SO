// concurrency.h
#ifndef CONCURRENCY_H
#define CONCURRENCY_H

#include "fscs.h"  // Inclui Caminho e outras estruturas definidas em fscs.h

// Função para inicializar memória partilhada para armazenar os caminhos de cidadãos
Caminho* inicializar_memoria_partilhada(int num_cidadaos);

// Função para inicializar os semáforos
void inicializar_semaforos();

// Função para bloquear o semáforo (garante acesso exclusivo à memória partilhada)
void lock_semaforo();

// Função para desbloquear o semáforo (libera o acesso à memória partilhada)
void unlock_semaforo();

// Função para liberar os recursos, como memória partilhada e semáforos
void liberar_recursos(Caminho *caminhos);

#endif // CONCURRENCY_H
