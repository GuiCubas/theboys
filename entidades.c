// TADs das entidades herói, base, missão e invasão
#include <stdlib.h>
#include "entidades.h"
#include "utils.h"
#include "mundo.h"


void inicializa_herois(mundo_t *w)
{
    heroi_t *h;
    
    for (int i = 0; i < N_HEROIS; i++) {
        h = w->herois[i];
        h->id = i;
        h->xp = 0;
        h->paciencia = aleat(0, 100);
        h->velocidade = aleat(50, 5000);
        h->habilidades = cjto_aleat(aleat(0,3), w->nHabilidades);
        h->estado = 1;
        h->vivo = 1;
    }
}

void inicializa_bases(mundo_t *w)
{
    base_t *b;

    for (int i = 0; i < N_BASES; i++) {
        b = w->bases[i];
        b->id = i;
        b->loc = coordenada_aleat(0, N_TAMANHO_MUNDO - 1);
        b->lotacao = aleat(3, 10);
        b->presentes = cjto_cria(b->lotacao);
        b->espera = fila_cria();
    }
}

void inicializa_missoes(mundo_t *w)
{
    missao_t *m;

    for (int i = 0; i < N_MISSOES; i++) {
        m = w->missoes[i];
        m->id = i;
        m->loc = coordenada_aleat(0, N_TAMANHO_MUNDO - 1);
        m->habilidades = cjto_aleat(aleat(6, 10), w->nHabilidades);
        m->perigo = aleat(0, 100);
    }
}

void inicializa_invasoes(mundo_t *w)
{
    invasao_t *inv;

    for (int i = 0; i < N_INVASOES; i++) {
        inv = w->invasoes[i];
        inv->id = i;
        inv->local = aleat(0, N_BASES - 1);
        inv->penalidadeHeroi = aleat(0, 4320);
        inv->penalidadeBase = aleat(0, 7200);
    }
}

base_t *encontra_BMP (mundo_t *w, missao_t *m)
{
    base_t *bases[w->nBases];
    int dists[w->nBases];

    for (int i = 0; i < w->nBases; i++) {
        bases[i] = w->bases[i];
        dists[i] = dist_cartesiana(bases[i]->loc, m->loc);
    }

    selectionSortDuplo(bases, dists, w->nBases);

    heroi_t *h;
    struct cjto_t *hab_totais = cjto_cria(w->nHabilidades);

    for (int i = 0; i < w->nBases; i++) {
        for (int j = 0; j < w->nHerois; j++) {
            h = w->herois[j];

            if (cjto_pertence(bases[i]->presentes, h->id) > 0 && h->estado)
                hab_totais = cjto_uniao(hab_totais, h->habilidades);
        }

        if(cjto_contem(hab_totais, m->habilidades))
            return bases[i];
    }

    return NULL;
}

base_t *base_aleat(mundo_t *w)
{
    int i = aleat(0, w->nBases);
    
    return w->bases[i];
}