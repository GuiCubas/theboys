#include "mundo.h"
#include "eventos.h"

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