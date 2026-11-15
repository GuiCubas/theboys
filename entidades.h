// TADs das entidades herói, base, missão e invasão

#ifndef ENTIDADES
#define ENTIDADES

#include "conjunto.h"
#include "fila.h"
#include "utils.h"

// HERÓI
typedef struct
{
    int id;                     // Identificador único do herói
    struct cjto_t *habilidades; // Conjunto de habilidades
    int estado;                // Estado do herói, ativo ou inativo
    int paciencia;              // Inteiro que representa a paciência do herói
    int velocidade;             // Inteiro que representa a velocidade do herói
    int xp;                     // Inteiro que representa o número de missões que o herói participou
    int base;                   // Identificador da base que o herói está
    int vivo;                  // Identificador se o herói está vivo ou morto
} heroi_t;

// BASE
typedef struct base
{
    int id;                     // Identificar único da base
    int lotacao;                // Max de heróis na base
    struct cjto_t *presentes;   // Conjunto dos heróis presentes
    int estado;                // Estado da base, ativa ou inativa
    struct fila_t *espera;      // Fila de espera para entrar na base
    coordenada_t loc;      // Par de coordenadas X e Y da base
} base_t;

// MISSÃO
typedef struct
{
    int id;                     // Identificador único da missão
    struct cjto_t *habilidades; // Conjunto de habilidades necessários para concluir a missão
    int perigo;                 // Nível de perigo da missão
    coordenada_t loc;      // Par de coordenadas X e Y da missão
} missao_t;

// INVASÃO
typedef struct
{
    int id;                     // Identificador único da invasão
    int local;                  // Base que será invadida
    int penalidadeBase;         // Tempo de penalidade que a base vai sofrer
    int penalidadeHeroi;        // Tempo de penalidade que os heróis da base vão sofrer
} invasao_t;

#endif