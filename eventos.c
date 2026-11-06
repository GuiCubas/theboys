#include <stdio.h>
#include "entidades.h"
#include "fprio.h"
#include "mundo.h"
#include "eventos.h"
#include "utils.h"

typedef struct
{
    void *campo1;       // Ponteiro genérico para campo genérico, normalmente herói
    void *campo2;       //--, normalmente base
} evento;

evento *cria_evento (void *campo1, void *campo2)
{
    //cria e inicializa o evento com esses campos ai
}

void chega (mundo *w, evento *ev)
{
    heroi *h = ev->campo1;
    base *b = ev->campo2;
    bool espera;

    h->base = b->id;

    espera = (!fprio_tamanho(b->espera)) ? true : h->paciencia > (10 * fprio_tamanho(b->espera));

    evento *novo_ev = cria_evento(h, b);

    int tipo = (espera && b->estado) ? ESPERA : DESISTE;
    fprio_insere(w->lef, novo_ev, tipo, w->relogio);
}

void espera (mundo *w, evento *ev)
{
    heroi *h = ev->campo1;
    base *b = ev->campo2;

    fila_insere (b->espera, h);

    evento *novo_ev = cria_evento(b, NULL);
    fprio_insere(w->lef, novo_ev, AVISA, w->relogio);
}

void desiste (mundo *w, evento *ev)
{
    heroi *h = ev->campo1;

    base *d = base_aleat();

    evento *novo_ev = cria_evento(h, d);
    fprio_insere(w->lef, novo_ev, VIAJA, w->relogio);
}

void avisa (mundo *w, evento *ev)
{
    base *b = ev->campo1;

    while (cjto_card(b->presentes) < b->lotacao) {
        heroi *h = fila_retira;
        cjto_insere(b->presentes, h->id);

        evento *novo_ev = cria_evento(h, b);
        fprio_insere(w->lef, novo_ev, ENTRA, w->relogio);
    }
}

void entra (mundo *w, evento *ev)
{
    heroi *h = ev->campo1;
    base *b = ev->campo2;

    // Calculo do tempo de permanência na base
    int tpb = 15 + h->paciencia * aleat(0, 20);

    evento *novo_ev = cria_evento(h, b);
    fprio_insere(w->lef, novo_ev, SAI, w->relogio + tpb);
}

