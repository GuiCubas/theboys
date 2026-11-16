#include <stdio.h>
#include <stdlib.h>
#include "mundo.h"
#include "entidades.h"
#include "eventos.h"
#include "utils.h"
#include "lef.h"
#include "fila.h"

/* ------------------------------------------------------------------ */
/* MUNDO                                                              */
/* ------------------------------------------------------------------ */

mundo_t *mundo_ini ()
{
    mundo_t *w = calloc(1, sizeof(mundo_t));

    w->nHerois = N_HEROIS;
    w->nBases = N_BASES;
    w->nMissoes = N_MISSOES;
    w->nInvasoes = N_INVASOES;
    w->nHabilidades = N_HABILIDADES;
    w->tamX = N_TAMANHO_MUNDO;
    w->tamY = N_TAMANHO_MUNDO;
    w->relogio = T_INICIO;
    w->lef = lef_cria();
    w->estats = estats_cria();

    herois_ini(w);
    bases_ini(w);
    missoes_ini(w);
    invasoes_ini(w);
    ini_ev(w);
    

    return w;
}

mundo_t *mundo_destroi (mundo_t *w)
{
    if (!w)
        return NULL;

    // HEROIS
    for (int i = 0; i < w->nHerois; i++) {
        if (w->herois[i]) {
            cjto_destroi(w->herois[i]->habilidades);
            free(w->herois[i]);
        }
    }
    free(w->herois);

    // BASES
    for (int i = 0; i < w->nBases; i++) {
        if (w->bases[i]) {
            cjto_destroi(w->bases[i]->presentes);
            fila_destroi(w->bases[i]->espera);
            free(w->bases[i]);
        }
    }
    free(w->bases);

    // MISSOES
    for (int i = 0; i < w->nMissoes; i++) {
        if (w->missoes[i]) {
            cjto_destroi(w->missoes[i]->habilidades);
            free(w->missoes[i]);
        }
    }
    free(w->missoes);

    // INVASOES
    for (int i = 0; i < w->nInvasoes; i++)
        free(w->invasoes[i]);
    free(w->invasoes);

    // TADs
    estats_destroi(w);
    lef_destroi(w->lef);
    free(w);

    return NULL;
}

/* ------------------------------------------------------------------ */
/* ENTIDADES                                                          */
/* ------------------------------------------------------------------ */

void herois_ini (mundo_t *w)
{
    w->herois = calloc(w->nHerois, sizeof(heroi_t *));

    for (int i = 0; i < w->nHerois; i++) {
        heroi_t *h = calloc(1, sizeof(heroi_t));
        w->herois[i] = h;

        h->id = i;
        h->xp = 0;
        h->paciencia = aleat(0, 100);
        h->velocidade = aleat(50, 5000);
        h->habilidades = cjto_aleat(aleat(1, 3), w->nHabilidades);
        h->estado = 1;
        h->vivo = 1;
    }
}

void bases_ini (mundo_t *w)
{
    w->bases = calloc(w->nBases, sizeof(base_t *));

    for (int i = 0; i < w->nBases; i++) {
        base_t *b = calloc(1, sizeof(base_t));
        w->bases[i] = b;

        b->id = i;
        b->loc = coordenada_aleat(0, N_TAMANHO_MUNDO - 1);
        b->lotacao = aleat(3, 10);
        b->presentes = cjto_cria(w->nHerois);
        b->espera = fila_cria();
        b->estado = 1;
    }
}

void missoes_ini (mundo_t *w)
{
    w->missoes = calloc(w->nMissoes, sizeof(missao_t *));

    for (int i = 0; i < w->nMissoes; i++) {
        missao_t *m = calloc(1, sizeof(missao_t));
        w->missoes[i] = m;
        m->id = i;
        m->loc = coordenada_aleat(0, N_TAMANHO_MUNDO - 1);
        m->habilidades = cjto_aleat(aleat(6, 10), w->nHabilidades);
        m->perigo = aleat(0, 100);
    }
}

void invasoes_ini (mundo_t *w)
{
    w->invasoes = calloc(w->nInvasoes, sizeof(invasao_t *));

    for (int i = 0; i < w->nInvasoes; i++) {
        invasao_t *inv = calloc(1, sizeof(invasao_t));
        w->invasoes[i] = inv;
        inv->id = i;
        inv->local = aleat(0, w->nBases - 1);
        inv->penalidadeHeroi = aleat(0, 4320);
        inv->penalidadeBase = aleat(0, 7200);
    }
}

/* ------------------------------------------------------------------ */
/* EVENTOS INICIAIS                                                   */
/* ------------------------------------------------------------------ */

void herois_ini_ev (mundo_t *w)
{
    for (int i = 0; i < w->nHerois; i++) {
        base_t *b = w->bases[aleat(0, w->nBases - 1)];
        int tempo = aleat(0, 4320);
        agenda_evento(w, w->herois[i], b, CHEGA, tempo);
    }
}

void missoes_ini_ev (mundo_t *w)
{
    for (int i = 0; i < w->nMissoes; i++) {
        int tempo = aleat(0, T_FIM_DO_MUNDO);
        agenda_evento(w, w->missoes[i], NULL, MISSAO, tempo);
    }
}

void invasoes_ini_ev (mundo_t *w)
{
    for (int i = 0; i < w->nInvasoes; i++) {
        int tempo = aleat(4321, T_FIM_DO_MUNDO);
        agenda_evento(w, w->invasoes[i], NULL, INVADE, tempo);
    }
}

void ini_ev (mundo_t *w)
{
    herois_ini_ev(w);
    missoes_ini_ev(w);
    invasoes_ini_ev(w);
    agenda_evento(w, NULL, NULL, FIM, T_FIM_DO_MUNDO);
}

/* ------------------------------------------------------------------ */
/* ESTATÍSTICAS                                                       */
/* ------------------------------------------------------------------ */

estats_t *estats_cria ()
{
    estats_t *estats = calloc(1, sizeof(estats_t));
    estats->m_tentativas = calloc(N_MISSOES, sizeof(int));
    estats->bases = calloc(N_BASES, sizeof(estatsBase_t *));

    for (int i = 0; i < N_BASES; i++)
        estats->bases[i] = calloc(1, sizeof(estatsBase_t));

    return estats;
}

estats_t *estats_destroi (mundo_t *w)
{
    for (int i = 0; i < w->nBases; i++)
        free(w->estats->bases[i]);
        
    free(w->estats->bases);
    free(w->estats->m_tentativas);
    free(w->estats);

    return NULL;
}

/* ------------------------------------------------------------------ */
/* SIMULAÇÃO                                                          */
/* ------------------------------------------------------------------ */

void simulacao (mundo_t *w)
{
    int tipo, tempo;
    evento_t *ev;

    while (w->relogio <= T_FIM_DO_MUNDO) {
        ev = lef_retira(w->lef, &tipo, &tempo);
        if (!ev)
            break;

        w->relogio = tempo;

        switch (tipo) {
            case CHEGA:
                chega(w, ev);
                break;

            case ESPERA:
                espera(w, ev);
                break;

            case DESISTE:
                desiste(w, ev);
                break;

            case AVISA:
                avisa(w, ev);
                break;

            case ENTRA:
                entra(w, ev);
                break;

            case SAI:
                sai(w, ev);
                break;

            case VIAJA:
                viaja(w, ev);
                break;

            case MORRE:
                morre(w, ev);
                break;

            case MISSAO:
                missao(w, ev);
                break;

            case INVADE:
                invade(w, ev);
                break;

            case RECUPERA_HEROI:
                recupera_heroi(ev);
                break;

            case RECONSTROI_BASE:
                reconstroi_base(ev);
                break;

            case FIM:
                fim(w, ev);
                break; 

            default:
                destroi_evento(ev);
                break;
        }
        w->estats->ev_totais++;

        if (tipo == FIM)        // Garantia q vai parar no FIM
            break;
    }
}