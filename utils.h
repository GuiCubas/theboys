#ifndef UTILS
#define UTILS

#include "mundo.h"
#include "utils.h"
#include "eventos.h"
#include "entidades.h"
// Declaração antecipada para o compilador saber que a struct existe
struct base;
typedef struct base base_t;

// Par de Coordenadas X e Y 
typedef struct
{
    int x;                        // Coordenada X no mundo
    int y;                      // Coordenada Y no mundo
} coordenada_t;

typedef struct
{
    int churata;
} estats_t;

int aleat (int min, int max);

coordenada_t coordenada_aleat (int min, int max);

int dist_cartesiana (coordenada_t loc1, coordenada_t loc2);

void selectionSortDuplo(base_t *v[], int u[], int tam);

#endif