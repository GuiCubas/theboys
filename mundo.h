// TAD que representa o mundo e suas funções

#ifndef MUNDO  
#define MUNDO

#include "entidades.h"
#include "lef.h"

#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600 //= 60*24*365 = 365 dias
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 5)
#define N_MISSOES (T_FIM_DO_MUNDO / 100)
#define N_INVASOES (T_FIM_DO_MUNDO / 10000)

typedef struct
{
    int nHerois;            // Número total de heróis no mundo
    heroi_t **herois;          // Vetor dos heróis no mundo

    int nBases;             // Número total de bases no mundo
    base_t **bases;            // Vetor das bases no mundo

    int nMissoes;           // Número total de missões
    missao_t **missoes;        // Vetor das missões

    int nInvasoes;          // Número total de invasões
    invasao_t **invasoes;      // Vetor das invasões

    int nHabilidades;       // Número de habilidades distintas possíveis 

    int tamX;               // Tamanho máximo do mundo no eixo X
    int tamY;               // Tamanho máximo do mundo no eixo Y

    int relogio;            // Relógio que indica o tempo atual
    
    struct lef_t *lef;
} mundo_t;

mundo_t *inicializa_mundo ();

void destroi_mundo (mundo_t *w);

void simulacao(mundo_t *w);

#endif