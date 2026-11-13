// TADs das entidades herói, base, missão e invasão

#ifndef ENTIDADES
#define ENTIDADES

#include <stdio.h>
#include <stdbool.h>
#include "conjunto.h"
#include "fila.h"
#include "utils.h"

// HERÓI
typedef struct
{
    int id;                     // Identificador único do herói
    struct cjto_t *habilidades; // Conjunto de habilidades
    bool estado;                // Estado do herói, ativo ou inativo
    int paciencia;              // Inteiro que representa a paciência do herói
    int velocidade;             // Inteiro que representa a velocidade do herói
    int xp;                     // Inteiro que representa o número de missões que o herói participou
    int base;                   // Identificador da base que o herói está
    bool vivo;                  // Identificador se o herói está vivo ou morto
} heroi_t;

// BASE
typedef struct
{
    int id;                     // Identificar único da base
    int lotacao;                // Max de heróis na base
    struct cjto_t *presentes;   // Conjunto dos heróis presentes
    bool estado;                // Estado da base, ativa ou inativa
    struct fila_t *espera;      // Fila de espera para entrar na base
    coordenada_t loc;      // Par de coordenadas X e Y da base
} base_t;

// MISSÃO
typedef struct
{
    int id;                     // Identificador único da missão
    struct cjto_t *habilidades; // Conjunto de habilidades necessários para concluir a missão
    int perigo;                 // Nível de perigo da missão
    int tentativas;             // Tentativas até a missão ser cumprida
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

base_t *base_aleat(mundo_t *w);

base_t *encontra_BMP(mundo_t *w, missao_t *m);

#endif