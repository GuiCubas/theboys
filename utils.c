#include <math.h>
#include <stdlib.h>
#include <utils.h>
#include "entidades.h"

// TROCAR PARA INSERTION
void selectionSortDuplo(base_t *v[], int u[], int tam)
{
    int i, j, auxU, auxV;
    
    for (j = 1; j < tam; j++) {
        auxU = u[j];
        auxV = v[j];
        i = j - 1;

        while ((i >= 0) && (u[i] > auxU)) {
            u[i + 1] = u[i];
            v[i + 1] = v[i];
            i = i - 1;
        }
        
        u[i + 1] = auxU;
        v[i + 1] = auxV;
    }
}

int aleat (int min, int max)
{
    return min + rand() % (max - min + 1);
}

coordenada_t coordenada_aleat (int min, int max)
{
    coordenada_t loc;
    loc.x = aleat(min, max);
    loc.y = aleat(min, max);

    return loc;
}

int dist_cartesiana (coordenada_t loc1, coordenada_t loc2)
{
    int distx = pow(loc2.x - loc1.x, 2.0);
    int disty = pow(loc2.y - loc1.y, 2.0);

    return sqrt(distx + disty);
}