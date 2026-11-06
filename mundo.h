// TAD que representa o mundo e suas funções

#ifndef MUNDO   
#define MUNDO

#include <stdio.h>
#include <stdbool.h>
#include "entidades.h"
#include "mundo.h"
#include "fprio.h"

typedef struct
{
    int nHerois;            // Número total de heróis no mundo
    heroi *herois;          // Vetor dos heróis no mundo

    int nBases;             // Número total de bases no mundo
    base *bases;            // Vetor das bases no mundo

    int nMissoes;           // Número total de missões
    missao *missoes;        // Vetor das missões

    int nInvasoes;          // Número total de invasões
    invasao *invasoes;      // Vetor das invasões

    int nHabilidades;       // Número de habilidades distintas possíveis 

    int tamX;               // Tamanho máximo do mundo no eixo X
    int tamY;               // Tamanho máximo do mundo no eixo Y

    int relogio;            // Relógio que indica o tempo atual
    
    struct fprio_t *lef;
} mundo;

void inicializaMundo (mundo mundo);

#endif