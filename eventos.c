#include <stdio.h>
#include <stdlib.h>
#include "entidades.h"
#include "lef.h"
#include "mundo.h"
#include "eventos.h"
#include "utils.h"

// Retorna ponteiro para evento criado com 2 campos
evento_t *cria_evento (void *campo1, void *campo2)
{
    evento_t *evento = malloc(sizeof(evento_t));

    evento->campo1 = campo1;
    evento->campo2 = campo2;

    return(evento);
}

// Libera a memória alocada para esse evento
void destroi_evento (evento_t *ev)
{
    free(ev);
}

void agenda_evento (mundo_t *w, void *campo1, void *campo2, int tipo, int time)
{
    evento_t *ev = cria_evento(campo1, campo2);

    lef_insere(w->lef, ev, tipo, w->relogio + time);
}

void chega (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    base_t *b = ev->campo2;
    int espera;

    h->base = b->id;

    espera = (!lef_tamanho(b->espera)) ? 1 : h->paciencia > (10 * lef_tamanho(b->espera));

    int tipo = (espera && b->estado) ? ESPERA : DESISTE;
    agenda_evento (w, h, b, tipo, AGORA);

    if (tipo = ESPERA)
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA", w->relogio, h->id, b->id, cjto_card(b->presentes), b->lotacao);
    else 
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE", w->relogio, h->id, b->id, cjto_card(b->presentes), b->lotacao);
    
    destroi_evento(ev);
}

void espera (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    base_t *b = ev->campo2;

    fila_insere (b->espera, h);

    agenda_evento(w, b, NULL, AVISA, AGORA);

    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)", w->relogio, h->id, b->id, fila_tamanho(b->espera));

    destroi_evento(ev);
}

void desiste (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    base_t *b = ev->campo2;
    base_t *d = base_aleat(w);

    agenda_evento(w, h, d, VIAJA, AGORA);

    printf("%6d: DESISTE HEROI %2d BASE %d", w->relogio, h->id, b->id);

    destroi_evento(ev);
}

void avisa (mundo_t *w, evento_t *ev)
{
    base_t *b = ev->campo1;

    //TEM Q VER ESSE COMENTARIO AI 

    while (cjto_card(b->presentes) < b->lotacao) {
        heroi_t *h = fila_retira(b->espera);
        cjto_insere(b->presentes, h->id);

        agenda_evento(w, h, b, ENTRA, AGORA);

        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d", w->relogio, b->id, h->id);
    }

    destroi_evento(ev);
}

void entra (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    base_t *b = ev->campo2;

    // Calculo do tempo de permanência na base
    int tpb = 15 + h->paciencia * aleat(0, 20);

    agenda_evento(w, h, b, SAI, tpb);

    printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d", w->relogio, h->id, b->id, cjto_card(b->presentes), b->presentes);

    destroi_evento(ev);
}

void sai (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    base_t *b = ev->campo2;

    cjto_retira(b->presentes, h->id);

    base_t *d = base_aleat(w);

    agenda_evento(w, h, d, VIAJA, AGORA);
    agenda_evento(w, b, NULL, AVISA, AGORA);

    printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)", w->relogio, h->id, b->id, cjto_card(b->presentes), b->presentes);

    destroi_evento(ev);
}

void viaja (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    base_t *d = ev->campo2;

    int dist = dist_cartesiana(w->bases[h->base]->loc, d->loc);
    int duracao = dist / h->velocidade;

    agenda_evento(w, h, d, CHEGA, duracao);

    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d", w->relogio, h->id, h->base, d, dist, h->velocidade, w->relogio + duracao);

    destroi_evento(ev);
}

void morre (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    missao_t *m = ev->campo2;
    base_t *b = &w->bases[h->base];

    cjto_retira(b->presentes, h->id);
    lef_retira_heroi(w->lef, h->id);

    h->vivo = 0;

    agenda_evento(w, b, NULL, AVISA, AGORA);

    printf("%6d: MORRE HEROI %2d MISSAO %d", w->relogio, h->id, m->id);

    destroi_evento(ev);
}

void missao (mundo_t *w, evento_t *ev)
{
    missao_t *m = ev->campo1;
    base_t *bmp = encontra_BMP (w, m);
    heroi_t *h;

    m->tentativas++;

    if (!bmp) {
        agenda_evento(w, m, NULL, MISSAO, 24*60);
        destroi_evento(ev);
        return;
    }

    int risco;

    for (int i = 0; i < w->nHerois; i++) {
        h = w->herois[i];
        
        if (cjto_pertence(bmp->presentes, h->id) > 0 && h->estado) {
            risco = m->perigo / (h->paciencia + h->xp + 1);

            (risco > aleat(0, 30)) ? agenda_evento(w, h, m, MORRE, AGORA) : h->xp++;
        }   
    }

    destroi_evento(ev);
}

void invade (mundo_t *w, evento_t *ev)
{
    invasao_t *inv = ev->campo1;
    base_t *b = w->bases[inv->local];
    heroi_t *h;

    if (!b->estado) {
        destroi_evento(ev);
        return;
    }

    for (int i = 0; i < w->nHerois; i++) {
        h = w->herois[i];
        if (cjto_pertence(b->presentes, h->id) > 0 && h->estado) {
            h->estado = 0;
            agenda_evento(w, h, NULL, RECUPERA_HEROI, inv->penalidadeHeroi);

            struct lefnodo_t *aux = w->lef->prim;
            struct lefnodo_t *aux2;

            while(aux->prox->prio <= T_FIM_DO_MUNDO) {
                if ((aux->prox->tipo == SAI) && (aux->prox->item->campo1 == w->herois[i])) {
                aux2 = aux->prox;
                aux->prox = aux2->prox;

                destroi_evento(aux2->item);
                free(aux2);
                }
                aux = aux->prox;
            }
            
            agenda_evento(w, h, b, SAI, AGORA);
        }
    }

    b->estado = 0;
    agenda_evento(w, b, NULL, reconstroi_base, inv->penalidadeBase);

    destroi_evento(ev);
}

void recupera_heroi (evento_t *ev)
{
    heroi_t *h = ev->campo1;
    h->estado = 1;

    destroi_evento(ev);
}

void reconstroi_base (evento_t *ev)
{
    base_t *b = ev->campo1;
    b->estado = 1;

    destroi_evento(ev);
}

void fim (mundo_t *w)
{
    printf("acabou");
}