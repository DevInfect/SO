#ifndef INPUT_H
#define INPUT_H

typedef struct {
    int avenida;
    int rua;
} Coordenada;

typedef struct {
    int M;  // Número de avenidas
    int N;  // Número de ruas
    int num_supermercados;
    int num_cidadaos;
    Coordenada *supermercados;
    Coordenada *cidadaos;
    int **grid;  // Matriz para representar o grid da cidade
} Mapa;

// Função para carregar o input a partir de um ficheiro
int load_input(const char *filename, Mapa *mapa);

// Função para liberar a memória do mapa
void free_mapa(Mapa *mapa);

#endif // INPUT_H
