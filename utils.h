#ifndef UTILS
#define UTILS

// Par de Coordenadas X e Y 
typedef struct
{
    int x;                        // Coordenada X no mundo
    int y;                        // Coordenada Y no mundo
} coordenada_t;

// Retorna um inteiro aleatório entre min e max
int aleat (int min, int max);

// Retorna um ponto de coordenadas X e Y, ambas aleatórias entre min e max
coordenada_t coordenada_aleat (int min, int max);

// Retorna a distância cartesiana entre dois pontos com coordenadas X e Y
int dist_cartesiana (coordenada_t loc1, coordenada_t loc2);

#endif