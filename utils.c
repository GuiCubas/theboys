#include <stdio.h>
#include "entidades.h"

void selectionSortDuplo(base_t *v[], int u[], int tam)
{
    int i, j, auxU, auxV;
    
    for (j = 1; j < tam; j++) {
        // Guarda os valores atuais de ambos os vetores
        auxU = u[j];
        auxV = v[j];
        i = j - 1;

        // Compara APENAS usando o vetor U
        while ((i >= 0) && (u[i] > auxU)) {
            u[i + 1] = u[i]; // Desloca em U
            v[i + 1] = v[i]; // Replica o deslocamento em V
            i = i - 1;
        }
        
        // Insere ambos na posição correta encontrada
        u[i + 1] = auxU;
        v[i + 1] = auxV;
    }
}