#ifndef UTILS
#define UTILS

// Declaração antecipada para o compilador saber que a struct existe

// Par de Coordenadas X e Y 
typedef struct
{
    int x;                        // Coordenada X no mundo
    int y;                      // Coordenada Y no mundo
} coordenada_t;

int aleat (int min, int max);

coordenada_t coordenada_aleat (int min, int max);

int dist_cartesiana (coordenada_t loc1, coordenada_t loc2);

#endif