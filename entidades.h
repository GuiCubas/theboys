// TADs das entidades herói, base, missão e invasão

#ifndef ENTIDADES
#define ENTIDADES

#include <stdio.h>
#include <stdbool.h>
#include "conjunto.h"
#include "fila.h"
#include "entidades.h"

// Par de Coordenadas X e Y 
struct coordenada
{
    int X;                      // Coordenada X no mundo
    int Y;                      // Coordenada Y no mundo
};

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
} heroi;

// BASE
typedef struct
{
    int id;                     // Identificar único da base
    int lotacao;                // Max de heróis na base
    struct cjto_t *presentes;   // Conjunto dos heróis presentes
    bool estado;                // Estado da base, ativa ou inativa
    struct fila_t *espera;      // Fila de espera para entrar na base
    struct coordenada loc;      // Par de coordenadas X e Y da base
} base;

// MISSÃO
typedef struct
{
    int id;                     // Identificador único da missão
    struct cjto_t *habilidades; // Conjunto de habilidades necessários para concluir a missão
    int perigo;                 // Nível de perigo da missão
    struct coordenada loc;      // Par de coordenadas X e Y da missão
} missao;

// INVASÃO
typedef struct
{
    int id;                     // Identificador único da invasão
    int local;                  // Base que será invadida
    int penalidadeBase;         // Tempo de penalidade que a base vai sofrer
    int penalidadeHeroi;        // Tempo de penalidade que os heróis da base vão sofrer
} invasao;

base *base_aleat();

#endif