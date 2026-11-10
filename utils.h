#ifndef UTILS
#define UTILS

// Par de Coordenadas X e Y 
typedef struct
{
    int X;                      // Coordenada X no mundo
    int Y;                      // Coordenada Y no mundo
} coordenada_t;


int aleat (min, max);


int dist_cartesiana (coordenada_t loc1, coordenada_t loc2);

#endif