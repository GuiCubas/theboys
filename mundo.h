// TAD que representa o mundo e suas funções

#ifndef MUNDO  
#define MUNDO

#include <stdio.h>
#include <stdbool.h>
#include "entidades.h"
#include "mundo.h"
#include "lef.h"

typedef struct
{
    int nHerois;            // Número total de heróis no mundo
    heroi_t *herois;          // Vetor dos heróis no mundo

    int nBases;             // Número total de bases no mundo
    base_t *bases;            // Vetor das bases no mundo

    int nMissoes;           // Número total de missões
    missao_t *missoes;        // Vetor das missões

    int nInvasoes;          // Número total de invasões
    invasao_t *invasoes;      // Vetor das invasões

    int nHabilidades;       // Número de habilidades distintas possíveis 

    int tamX;               // Tamanho máximo do mundo no eixo X
    int tamY;               // Tamanho máximo do mundo no eixo Y

    int relogio;            // Relógio que indica o tempo atual
    
    struct lef_t *lef;
} mundo_t;

void inicializaMundo (mundo_t mundo);

#endif