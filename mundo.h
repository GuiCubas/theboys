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

// Estatísticas de cada base
typedef struct
{
    int fila_max;           // Quantidade máxima de H q ficaram na fila
    int m_cumpridas;        // Missões cumpridas
} estatsBase_t;

// Estatísticas gerais do mundo 
typedef struct
{
    int ev_totais;          // Total de eventos tratados
    int m_cumpridas;        // Total de missões cumpridas
    int *m_tentativas;      // Vetor de tentativas de cada missão, cada índice uma missão
    int inv_realizadas;     // Total de invasões realizadas
    int mortes;             // Total de mortes ocorridas
    estatsBase_t **bases;   // Vetor de estatísticas das bases, cada índice uma base
} estats_t;

// MUNDO
typedef struct mundo
{
    int nHerois;            // Número total de heróis no mundo
    heroi_t **herois;       // Vetor dos heróis no mundo

    int nBases;             // Número total de bases no mundo
    base_t **bases;         // Vetor das bases no mundo

    int nMissoes;           // Número total de missões
    missao_t **missoes;     // Vetor das missões

    int nInvasoes;          // Número total de invasões
    invasao_t **invasoes;   // Vetor das invasões

    int nHabilidades;       // Número de habilidades distintas possíveis 

    int tamX;               // Tamanho máximo do mundo no eixo X
    int tamY;               // Tamanho máximo do mundo no eixo Y

    int relogio;            // Relógio que indica o tempo atual
    
    struct lef_t *lef;      // Fila de prioridades do mundo

    estats_t *estats;       // Estatísticas do mundo
} mundo_t;

// Inicializa o mundo e todos seus campos
mundo_t *mundo_ini ();

// Libera toda a memoria alocada do mundo e seus campos
mundo_t *mundo_destroi (mundo_t *w);

// Inicializa os heróis do mundo
void herois_ini (mundo_t *w);

// Agenda os eventos iniciais dos heróis
void herois_ini_ev (mundo_t *w);

// Inicializa as bases do mundo
void bases_ini (mundo_t *w);

// Inicializa as missões do mundo
void missoes_ini (mundo_t *w);

// Agenda os eventos iniciais das missões
void missoes_ini_ev (mundo_t *w);

// Inicializa as invasões do mundo
void invasoes_ini (mundo_t *w);

// Agenda os eventos iniciais das invasões
void invasoes_ini_ev (mundo_t *w);

// Agenda os eventos iniciais do mundo
void ini_ev (mundo_t *w);

// Inicializa as estatística do mundo e todos seus campos
estats_t *estats_cria ();

// Libera toda a memoria alocada das estats e seus campos
estats_t *estats_destroi (mundo_t *w);

// Simulação dos eventos, leitura de cada evento da LEF
void simulacao(mundo_t *w);

#endif