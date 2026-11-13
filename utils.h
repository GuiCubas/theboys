#ifndef UTILS
#define UTILS

// Par de Coordenadas X e Y 
typedef struct
{
    int X;                      // Coordenada X no mundo
    int Y;                      // Coordenada Y no mundo
} coordenada_t;

typedef struct
{

} estats_t;

int aleat (min, max);

coordenada_t coordenada_aleat (min, max);

int dist_cartesiana (coordenada_t loc1, coordenada_t loc2);

void selectionSortDuplo(base_t *v[], int u[], int tam);

#endif