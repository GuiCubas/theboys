#include <stdio.h>
#include <stdlib.h>
#include "mundo.h"
#include "entidades.h"
#include "eventos.h"
#include "utils.h"
#include "fila.h"

void destroi_mundo (mundo_t *w)
{
    free(w->herois);
    free(w->bases);
    free(w->missoes);
    free(w->invasoes);
    lef_destroi(w->lef);
    free(w);
}

mundo_t *inicializa_mundo ()
{
    mundo_t *w = malloc(sizeof(mundo_t));

    inicializa_herois(w);
    inicializa_bases(w);
    inicializa_missoes(w);
    inicializa_invasoes(w);
    inicializa_eventos(w);
    agenda_evento(w, NULL, NULL, FIM, T_FIM_DO_MUNDO);

    return w;
}

void inicializa_eventos (mundo_t *w)
{
    inicializa_eventos_herois(w);
    inicializa_eventos_missoes(w);
    inicializa_eventos_invasoes(w);
}

void inicializa_eventos_herois(mundo_t *w)
{
    base_t *b;
    int time;

    for (int i = 0; i < N_HEROIS; i++) {
        b = base_aleat;
        time = aleat(0, 4320);  //4320 = 60*24*3 = 3 dias
        agenda_evento(w, w->herois[i], b, CHEGA, time);
    }
}

void inicializa_eventos_missoes(mundo_t *w)
{
    int time;

    for (int i = 0; i < N_MISSOES; i++) {
        time = aleat (0, T_FIM_DO_MUNDO);
        agenda_evento(w, w->missoes[i], NULL, MISSAO, time);
    }
}

void inicializa_eventos_invasoes(mundo_t *w)
{
    int time;

    for (int i = 0; i < N_INVASOES; i++) {
        time = aleat ((60*24*3), T_FIM_DO_MUNDO);
        agenda_evento(w, w->invasoes[i], NULL, INVADE, time);
    }
}

void simulacao(mundo_t *w)
{
    int *tipo;
    int *time;
    struct lefnodo_t *ev;

    while (w->relogio <= T_FIM_DO_MUNDO)
    {
        ev = lef_retira(w->lef, tipo, time);
        w->relogio;

        switch (*tipo)
        {
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
                fim(w);
                break;
        }
        
        destroi_evento(ev);
    }
}