#include <stdio.h>
#include <stdlib.h>
#include "entidades.h"
#include "lef.h"
#include "mundo.h"
#include "eventos.h"
#include "utils.h"

typedef struct
{
    void *campo1;       // Ponteiro genérico para campo genérico, normalmente herói
    void *campo2;       //---, normalmente base
} evento_t;

// Retorna ponteiro para evento criado com 2 campos
evento_t *cria_evento (void *campo1, void *campo2)
{
    evento_t *evento = malloc(sizeof(evento_t));

    evento->campo1 = campo1;
    evento->campo2 = campo2;

    return(evento);
}

// Libera a memória alocada para esse evento
void destroi_evento (evento_t *evento)
{
    free(evento);
}

void chega (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    base_t *b = ev->campo2;
    bool espera;

    h->base = b->id;

    espera = (!lef_tamanho(b->espera)) ? true : h->paciencia > (10 * lef_tamanho(b->espera));

    evento_t *novo_ev = cria_evento(h, b);

    int tipo = (espera && b->estado) ? ESPERA : DESISTE;
    lef_insere(w->lef, novo_ev, tipo, w->relogio);

    destroi_evento(ev);
}

void espera (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    base_t *b = ev->campo2;

    fila_insere (b->espera, h);

    evento_t *novo_ev = cria_evento(b, NULL);
    lef_insere(w->lef, novo_ev, AVISA, w->relogio);

    destroi_evento(ev);
}

void desiste (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    base_t *d = base_aleat();

    evento_t *novo_ev = cria_evento(h, d);
    lef_insere(w->lef, novo_ev, VIAJA, w->relogio);

    destroi_evento(ev);
}

void avisa (mundo_t *w, evento_t *ev)
{
    base_t *b = ev->campo1;

    while (cjto_card(b->presentes) < b->lotacao) {
        heroi_t *h = fila_retira(b->espera);
        cjto_insere(b->presentes, h->id);

        evento_t *novo_ev = cria_evento(h, b);
        lef_insere(w->lef, novo_ev, ENTRA, w->relogio);
    }

    destroi_evento(ev);
}

void entra (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    base_t *b = ev->campo2;

    // Calculo do tempo de permanência na base
    int tpb = 15 + h->paciencia * aleat(0, 20);

    evento_t *novo_ev = cria_evento(h, b);
    lef_insere(w->lef, novo_ev, SAI, w->relogio + tpb);

    destroi_evento(ev);
}

void sai (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    base_t *b = ev->campo2;

    cjto_retira(b->presentes, h->id);

    base_t *d = base_aleat();
    
    evento_t *novo_ev = cria_evento(h, d);
    lef_insere(w->lef, novo_ev, VIAJA, w->relogio);

    evento_t *novo_ev2 = cria_evento(b, NULL);
    lef_insere(w->lef, novo_ev2, AVISA, w->relogio);

    destroi_evento(ev);
}

void viaja (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    base_t *d = ev->campo2;

    int dist = dist_cartesiana(w->bases[h->base].loc, d->loc);
    int duracao = dist / h->velocidade;

    evento_t *novo_ev = cria_evento(h, d);
    lef_insere(w->lef, novo_ev, CHEGA, w->relogio + duracao);

    destroi_evento(ev);
}

void morre (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    base_t *b = &w->bases[h->base];

    cjto_retira(b->presentes, h->id);
    lef_retira_heroi(w->lef, h->id);

    h->vivo = false;

    evento_t *novo_ev = cria_evento(b, NULL);
    lef_insere(w->lef, novo_ev, AVISA, w->relogio);

    destroi_evento(ev);
}

void missao (mundo_t *w, evento_t *ev)
{
    missao_t *m = ev->campo1;
    base_t *bmp = encontra_BMP (m, w);

    m->tentativas++;

    if (!bmp) {
        evento_t *novo_ev = cria_evento(m, NULL);
        lef_insere(w->lef, novo_ev, MISSAO, w->relogio + 24*60);
        destroi_evento(ev);
        return;
    }

    int risco;

    for (int i; i < w->nHerois; i++) {
        heroi_t *h = &w->herois[i];
        
        if (cjto_pertence(bmp->presentes, h->id) > 0 && h->estado) {
            risco = m->perigo / (h->paciencia + h->xp + 1);

            if (risco > aleat(0, 30)) {
                evento_t *novo_ev2 = cria_evento(h, NULL);
                lef_insere(w->lef, novo_ev2, MORRE, w->relogio);
            }

            else 
                h->xp++;
        }   
    }

    destroi_evento(ev);
}

void invade (mundo_t *w, evento_t *ev)
{
    invasao_t *inv = ev->campo1;
    base_t *b = &w->bases[inv->local];  // Perguntar se passa como campo 2 ou faz assim

    if (!b->estado) {
        destroi_evento(ev);
        return;
    }


}