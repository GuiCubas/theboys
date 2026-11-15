#include <stdio.h>
#include <stdlib.h>
#include "entidades.h"
#include "lef.h"
#include "mundo.h"
#include "eventos.h"
#include "utils.h"

base_t *encontra_BMP (mundo_t *w, missao_t *m);
base_t *base_aleat(mundo_t *w);
void insertionSortDuplo(base_t *v[], int u[], int tam);

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

// ATENCAO: confira em lef.h a ordem dos parametros de lef_insere.
// Aqui assumo (lef, item, tipo, prioridade/tempo). Se for (lef, item, tempo, tipo),
// troque a ordem na chamada abaixo.
void agenda_evento (mundo_t *w, void *campo1, void *campo2, int tipo, int time)
{
    evento_t *ev = cria_evento(campo1, campo2);

    lef_insere(w->lef, ev, tipo, w->relogio + time);
}

void chega (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    base_t *b = ev->campo2;
    int vai_esperar;

    h->base = b->id;

    if (cjto_card(b->presentes) < b->lotacao && !fila_tamanho(b->espera))
        vai_esperar = 1;
    else
        vai_esperar = h->paciencia > (10 * fila_tamanho(b->espera));

    int tipo = (vai_esperar && b->estado) ? ESPERA : DESISTE;
    agenda_evento (w, h, b, tipo, AGORA);

    printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) %s\n", w->relogio, h->id, b->id,
    cjto_card(b->presentes), b->lotacao, (tipo == ESPERA) ? "ESPERA" : "DESISTE");

    destroi_evento(ev);
}

void espera (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    base_t *b = ev->campo2;

    // Imprime antes de inserir: o tamanho nao conta o heroi que esta entrando
    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", w->relogio, h->id, b->id, fila_tamanho(b->espera));

    fila_insere(b->espera, h->id);

    agenda_evento(w, b, NULL, AVISA, AGORA);

    if (w->estats->bases[b->id]->fila_max < fila_tamanho(b->espera))
        w->estats->bases[b->id]->fila_max = fila_tamanho(b->espera);

    destroi_evento(ev);
}

void desiste (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    base_t *b = ev->campo2;
    base_t *d = base_aleat(w);

    agenda_evento(w, h, d, VIAJA, AGORA);

    printf("%6d: DESISTE HEROI %2d BASE %d\n", w->relogio, h->id, b->id);

    destroi_evento(ev);
}

void avisa (mundo_t *w, evento_t *ev)
{
    base_t *b = ev->campo1;

    printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA [ ", w->relogio, b->id, cjto_card(b->presentes), b->lotacao);
    fila_imprime(b->espera);
    printf(" ]\n");

    // So admite se a base esta de pe, ha vaga E ha alguem na fila
    while (b->estado && cjto_card(b->presentes) < b->lotacao && fila_tamanho(b->espera) > 0) {
        heroi_t *h = w->herois[fila_retira(b->espera)];
        cjto_insere(b->presentes, h->id);

        agenda_evento(w, h, b, ENTRA, AGORA);

        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", w->relogio, b->id, h->id);
    }

    destroi_evento(ev);
}

void entra (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    base_t *b = ev->campo2;

    // Tempo de permanência na base (aleat(1,20) conforme o enunciado)
    int tpb = 15 + h->paciencia * aleat(1, 20);

    agenda_evento(w, h, b, SAI, tpb);

    printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n", w->relogio, h->id, b->id,
           cjto_card(b->presentes), b->lotacao, w->relogio + tpb);

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

    printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", w->relogio, h->id, b->id,
           cjto_card(b->presentes), b->lotacao);

    destroi_evento(ev);
}

void viaja (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    base_t *d = ev->campo2;

    int dist = dist_cartesiana(w->bases[h->base]->loc, d->loc);
    int duracao = dist / h->velocidade;

    agenda_evento(w, h, d, CHEGA, duracao);

    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", w->relogio, h->id,
    h->base, d->id, dist, h->velocidade, w->relogio + duracao);

    destroi_evento(ev);
}

void morre (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    missao_t *m = ev->campo2;
    base_t *b = w->bases[h->base];

    cjto_retira(b->presentes, h->id);
    lef_retira_heroi(w->lef, h);

    h->vivo = 0;

    agenda_evento(w, b, NULL, AVISA, AGORA);

    printf("%6d: MORRE HEROI %2d MISSAO %d\n", w->relogio, h->id, m->id);

    w->estats->mortes++;
    destroi_evento(ev);
}

void missao (mundo_t *w, evento_t *ev)
{
    missao_t *m = ev->campo1;
    base_t *bmp = encontra_BMP (w, m);
    heroi_t *h;

    w->estats->m_tentativas[m->id]++;

    if (!bmp) {
        agenda_evento(w, m, NULL, MISSAO, 24*60);
        destroi_evento(ev);
        return;
    }

    for (int i = 0; i < w->nHerois; i++) {
        h = w->herois[i];
        
        if (cjto_pertence(bmp->presentes, h->id) > 0 && h->estado) {
            int risco = m->perigo / (h->paciencia + h->xp + 1);

            (risco > aleat(0, 30)) ? agenda_evento(w, h, m, MORRE, AGORA) : h->xp++;
        }   
    }

    w->estats->bases[bmp->id]->m_cumpridas++;
    w->estats->m_cumpridas++;
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

            // Varredura segura da LEF
            struct lefnodo_t *aux = w->lef->prim;

            while (aux != NULL && aux->prox != NULL) {
                // Verifica se o evento é do tipo SAI e pertence ao herói h
                if ((aux->prox->tipo == SAI) && (aux->prox->item->campo1 == h)) {
                    struct lefnodo_t *aux2 = aux->prox;
                    aux->prox = aux2->prox; // Remove da lista

                    destroi_evento(aux2->item);
                    free(aux2);
                    // IMPORTANTE: Não avançamos 'aux' aqui, pois o novo aux->prox 
                    // precisa ser verificado na próxima volta do loop.
                } else {
                    aux = aux->prox; // Só avança se não removeu
                }
            }
            
            agenda_evento(w, h, b, SAI, AGORA);
        }
    }

    b->estado = 0;
    agenda_evento(w, b, NULL, RECONSTROI_BASE, inv->penalidadeBase);

    w->estats->inv_realizadas++;
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

void fim (mundo_t *w, evento_t *ev)
{
    printf("%6d: FIM\n", w->relogio);

    for (int i = 0; i < w->nHerois; i++) {
        printf("HEROI %2d %s PAC %3d VEL %4d EXP %4d HABS [ ", w->herois[i]->id, (w->herois[i]->vivo) ? "VIVO" : "MORTO", 
        w->herois[i]->paciencia, w->herois[i]->velocidade, w->herois[i]->xp);
        cjto_imprime(w->herois[i]->habilidades);
        printf(" ]\n");
    }

    for (int i = 0; i < w->nBases; i++)
        printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", w->bases[i]->id, w->bases[i]->lotacao, w->estats->bases[i]->fila_max, w->estats->bases[i]->m_cumpridas);

    printf("EVENTOS TRATADOS: %d\n", w->estats->ev_totais);

    float porcentoMissao = (w->estats->m_cumpridas * 1.0 / w->nMissoes) * 100;
    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", w->estats->m_cumpridas, w->nMissoes, porcentoMissao);

    float porcentoInvasao = (w->estats->inv_realizadas * 1.0 / w->nInvasoes) * 100;
    printf("INVASÕES REALIZADAS: %d/%d (%.1f%%)\n", w->estats->inv_realizadas, w->nInvasoes, porcentoInvasao);

    int min = w->estats->m_tentativas[0];
    int max = w->estats->m_tentativas[0];
    int soma = 0;
    for (int i = 0; i < w->nMissoes; i++) {
        if (w->estats->m_tentativas[i] < min)
            min = w->estats->m_tentativas[i];

        if (w->estats->m_tentativas[i] > max)
            max = w->estats->m_tentativas[i];

        soma += w->estats->m_tentativas[i];;
    }
    
    float media = soma * 1.0 / w->nMissoes;
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", min, max, media);

    float mortalidade = (w->estats->mortes * 1.0 / w->nHerois) * 100;
    printf("TAXA MORTALIDADE: %.1f%%\n", mortalidade);

    destroi_evento(ev);
}

// AUXILIARES:
base_t *encontra_BMP (mundo_t *w, missao_t *m)
{
    base_t *bases[w->nBases];
    int dists[w->nBases];
 
    for (int i = 0; i < w->nBases; i++) {
        bases[i] = w->bases[i];
        dists[i] = dist_cartesiana(bases[i]->loc, m->loc);
    }
 
    insertionSortDuplo(bases, dists, w->nBases);
 
    heroi_t *h;
 
    for (int i = 0; i < w->nBases; i++) {
        // Conjunto de habilidades NOVO para cada base
        struct cjto_t *hab_totais = cjto_cria(w->nHabilidades);
 
        for (int j = 0; j < w->nHerois; j++) {
            h = w->herois[j];
 
            if (cjto_pertence(bases[i]->presentes, h->id) > 0 && h->estado) {
                struct cjto_t *novo = cjto_uniao(hab_totais, h->habilidades);
                cjto_destroi(hab_totais);
                hab_totais = novo;
            }
        }
 
        int ok = cjto_contem(hab_totais, m->habilidades);
        cjto_destroi(hab_totais);
 
        if (ok)
            return bases[i];
    }
 
    return NULL;
}

base_t *base_aleat(mundo_t *w)
{
    int i = aleat(0, w->nBases - 1);

    return w->bases[i];
}

void insertionSortDuplo(base_t *v[], int u[], int tam)
{
    int i, j, auxU;
    base_t *auxV;

    for (j = 1; j < tam; j++) {
        auxU = u[j];
        auxV = v[j];
        i = j - 1;

        while ((i >= 0) && (u[i] > auxU)) {
            u[i + 1] = u[i];
            v[i + 1] = v[i];
            i--;
        }
        u[i + 1] = auxU;
        v[i + 1] = auxV;
    }
}