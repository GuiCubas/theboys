#ifndef EVENTOS
#define EVENTOS

#include "mundo.h"
#include "entidades.h"
#define AGORA 0
#define CHEGA 1
#define ESPERA 2
#define DESISTE 3
#define AVISA 4
#define ENTRA 5
#define SAI 6
#define VIAJA 7
#define MORRE 8
#define MISSAO 9
#define INVADE 10
#define RECUPERA_HEROI 11
#define RECONSTROI_BASE 12
#define FIM 13

typedef struct
{
    void *campo1;       // Ponteiro genérico para campo genérico, normalmente herói
    void *campo2;       //---, normalmente base
} evento_t;

void agenda_evento (mundo_t *w, void *campo1, void *campo2, int tipo, int time);
void destroi_evento (evento_t *ev);
void chega (mundo_t *w, evento_t *ev);
void espera (mundo_t *w, evento_t *ev);
void desiste (mundo_t *w, evento_t *ev);
void avisa (mundo_t *w, evento_t *ev);
void entra (mundo_t *w, evento_t *ev);
void sai (mundo_t *w, evento_t *ev);
void viaja (mundo_t *w, evento_t *ev);
void morre (mundo_t *w, evento_t *ev);
void missao (mundo_t *w, evento_t *ev);
void invade (mundo_t *w, evento_t *ev);
void recupera_heroi (evento_t *ev);
void reconstroi_base (evento_t *ev);
void fim (mundo_t *w);

#endif