#include <stdio.h>
#include <stdlib.h>
#include "eventos.h"
#include "entidades.h"
#include "lef.h"
#include "mundo.h"
#include "utils.h"

// Retorna ponteiro para evento criado com 2 campos
evento_t *cria_evento (void *campo1, void *campo2);

// Encontra base mais próxima apta para realizar uma missão M
base_t *encontra_BMP (mundo_t *w, missao_t *m, struct cjto_t **hab);

// Sorteia uma base aleatória no vetor de bases do mundo
base_t *base_aleat (mundo_t *w);

// Insertion Sort de dois vetores ao mesmo tempo, usando como base o U para ordenar
void insertionSortDuplo (base_t *v[], int u[], int tam);

/* ------------------------------------------------------------------ */
/* AGENDAMENTO E DESTRUIÇÃO DE EVENTOS                                */
/* ------------------------------------------------------------------ */

evento_t *cria_evento (void *campo1, void *campo2)
{
    evento_t *evento = malloc(sizeof(evento_t));

    evento->campo1 = campo1;
    evento->campo2 = campo2;

    return(evento);
}

evento_t *destroi_evento (evento_t *ev)
{
    free(ev);
    return NULL;
}

void agenda_evento (mundo_t *w, void *campo1, void *campo2, int tipo, int time)
{
    evento_t *ev = cria_evento(campo1, campo2);

    lef_insere(w->lef, ev, tipo, w->relogio + time);
}

/* ------------------------------------------------------------------ */
/* OS EVENTOS EM SI                                                   */
/* ------------------------------------------------------------------ */

void chega (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    base_t *b = ev->campo2;
    int vai_esperar;

    // Troca a base de H
    h->base = b->id;

    // Se tiver vaga e fila vazia ele espera direto
    if (cjto_card(b->presentes) < b->lotacao && !fila_tamanho(b->espera))
        vai_esperar = 1;
    
    // Se não faz essa equação para decidir
    else
        vai_esperar = h->paciencia > (10 * fila_tamanho(b->espera));

    int tipo = (vai_esperar && b->estado) ? ESPERA : DESISTE;
    agenda_evento (w, h, b, tipo, AGORA);

    // Uso uma string pro tipo do evento, q é decidida pelo ternário
    printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) %s\n", w->relogio, h->id, b->id,
    cjto_card(b->presentes), b->lotacao, (tipo == ESPERA) ? "ESPERA" : "DESISTE");

    destroi_evento(ev);
}

void espera (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    base_t *b = ev->campo2;

    // Imprimo no começo pq o tamanho não conta o H atual
    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", w->relogio, h->id, b->id, fila_tamanho(b->espera));

    fila_insere(b->espera, h->id);

    agenda_evento(w, b, NULL, AVISA, AGORA);

    // Condicional pra ver o tamanho máximo da fila
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


    while (cjto_card(b->presentes) < b->lotacao && fila_tamanho(b->espera) > 0) {
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

    // Tempo de permanência na base
    int tpb = 15 + h->paciencia * aleat(1, 20);

    agenda_evento(w, h, b, SAI, tpb);

    printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n", w->relogio, 
    h->id, b->id, cjto_card(b->presentes), b->lotacao, w->relogio + tpb);

    destroi_evento(ev);
}

void sai (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    base_t *b = ev->campo2;
    base_t *d = base_aleat(w);

    cjto_retira(b->presentes, h->id);

    agenda_evento(w, h, d, VIAJA, AGORA);
    agenda_evento(w, b, NULL, AVISA, AGORA);

    printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", w->relogio, 
    h->id, b->id, cjto_card(b->presentes), b->lotacao);

    destroi_evento(ev);
}

void viaja (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    base_t *d = ev->campo2;

    int dist = dist_cartesiana(w->bases[h->base]->loc, d->loc);
    int duracao = dist / h->velocidade;

    agenda_evento(w, h, d, CHEGA, duracao);

    printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", w->relogio, 
    h->id, h->base, d->id, dist, h->velocidade, w->relogio + duracao);

    destroi_evento(ev);
}

void morre (mundo_t *w, evento_t *ev)
{
    heroi_t *h = ev->campo1;
    missao_t *m = ev->campo2;           // Passo a missao no campo do evento pra usar no print
    base_t *b = w->bases[h->base];

    cjto_retira(b->presentes, h->id);
    lef_limpeza_heroi(w->lef, h, 0);    // Passo 0 para retirar todos os eventos
    h->vivo = 0;

    agenda_evento(w, b, NULL, AVISA, AGORA);

    printf("%6d: MORRE HEROI %2d MISSAO %d\n", w->relogio, h->id, m->id);

    w->estats->mortes++;
    destroi_evento(ev);
}

void missao (mundo_t *w, evento_t *ev)
{
    missao_t *m = ev->campo1;
    struct cjto_t *habs = NULL;                 // cjto de habs usadas pela base, pra usar no print
    base_t *bmp = encontra_BMP (w, m, &habs);
    heroi_t *h;

    w->estats->m_tentativas[m->id]++;

    printf("%6d: MISSAO %d TENT %d HAB REQ: [ ", w->relogio, m->id, w->estats->m_tentativas[m->id]);
    cjto_imprime(m->habilidades);
    printf(" ]\n");

    // Se não achou nenhuma base
    if (!bmp) {
        agenda_evento(w, m, NULL, MISSAO, 24*60);
        printf("%6d: MISSAO %d IMPOSSIVEL\n", w->relogio, m->id);
        destroi_evento(ev);
        return;
    }

    for (int i = 0; i < w->nHerois; i++) {
        h = w->herois[i];
        
        if (cjto_pertence(bmp->presentes, h->id) > 0 && h->estado) {
            int risco = m->perigo / (h->paciencia + h->xp + 1);

            // Ternário q verifica se o H morre ou não
            (risco > aleat(0, 30)) ? agenda_evento(w, h, m, MORRE, AGORA) : h->xp++;
        }   
    }

    w->estats->bases[bmp->id]->m_cumpridas++;
    w->estats->m_cumpridas++;

    printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [ ", w->relogio, m->id, bmp->id);
    cjto_imprime(habs);
    printf(" ]\n");

    cjto_destroi(habs);     // Tem q destruir o cjto de habs da usados pela base

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

            lef_limpeza_heroi(w->lef, h, SAI);      // Retira só um evento do tipo SAI desse H
            agenda_evento(w, h, b, SAI, AGORA);
        }
    }

    b->estado = 0;
    agenda_evento(w, b, NULL, RECONSTROI_BASE, inv->penalidadeBase);

    w->estats->inv_realizadas++;

    printf("%6d: INVADE BASE %d INVADE %d\n", w->relogio, b->id, inv->id);
    printf("%6d: INVADE %d BASE %d HEROIS INATIVOS [ ", w->relogio, inv->id, b->id);
    cjto_imprime(b->presentes);
    printf(" ]\n");
    printf("%6d: INVADE %d BASE %d INATIVA\n", w->relogio, inv->id, b->id);

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

/* ------------------------------------------------------------------ */
/* AXULIARES                                                          */
/* ------------------------------------------------------------------ */

base_t *encontra_BMP (mundo_t *w, missao_t *m, struct cjto_t **habs)
{
    base_t *bases[w->nBases];
    int dists[w->nBases];
 
    // Inializa os dois vetores, um de bases e um com suas respectivas distâncias até a missão
    for (int i = 0; i < w->nBases; i++) {
        bases[i] = w->bases[i];
        dists[i] = dist_cartesiana(bases[i]->loc, m->loc);
    }
 
    // Ordenada os dois vetores com base no de dists
    insertionSortDuplo(bases, dists, w->nBases);
 
    heroi_t *h;
 
    for (int i = 0; i < w->nBases; i++) {
        struct cjto_t *hab_totais = cjto_cria(w->nHabilidades);
 
        for (int j = 0; j < w->nHerois; j++) {
            h = w->herois[j];
 
            if (cjto_pertence(bases[i]->presentes, h->id) > 0 && h->estado) {
                struct cjto_t *temp = cjto_uniao(hab_totais, h->habilidades);
                cjto_destroi(hab_totais);
                hab_totais = temp;          // Usa esse temp pra poder destruir o hab totais antigos, já q o união cria um cjto novo
            }
        }

        if (cjto_contem(hab_totais, m->habilidades) && bases[i]->estado) { 
            *habs = hab_totais;                 // Habs da base selecionada, pra usar no print
            return bases[i];
        }
        cjto_destroi(hab_totais);
    }
 
    // Se não achou nenhuma base, retorna NULL
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