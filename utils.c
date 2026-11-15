#include <math.h>
#include <stdlib.h>
#include "utils.h"

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