// TAD Fila de prioridades (FPRIO) genérica
// Carlos Maziero, DINF/UFPR, Out 2024
// Marcos Castilho, inclui as structs no arquivo.c, Out 2025
// Implementação com lista encadeada simples

// descreve um nodo da fila de prioridades
// NAO altere estas estruturas

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lef.h"
#include "entidades.h"
#include "eventos.h"

// Cria uma fila vazia.
struct lef_t *lef_cria ()
{
  struct lef_t *fila = malloc (sizeof(struct lef_t));

  if (!fila)
    return NULL;
  
  memset(fila, 0, sizeof(struct lef_t));
  
  return fila;
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
struct lef_t *lef_destroi (struct lef_t *f)
{
  if (!f)
    return NULL;

  struct lefnodo_t *aux = f->prim;
  struct lefnodo_t *prox;

  // Da free em cada nodo
  while (aux) {
    prox = aux->prox;
    free(aux->item);
    free(aux);
    aux = prox;
  }

  free(f);
  return NULL;
}

// Insere o item na fila, mantendo-a ordenada por prioridades crescentes.
int lef_insere (struct lef_t *f, void *item, int tipo, int prio)
{
  if (!f || !item)
    return -1;

  // Inicializa o novo nodo
  struct lefnodo_t *nodo = malloc (sizeof(struct lefnodo_t));
  if (!nodo)
    return -1;

  nodo->item = item;
  nodo->tipo = tipo;
  nodo->prio = prio;
  nodo->prox = NULL;

  if (!f->num)
    f->prim = nodo;

  else {
    struct lefnodo_t *aux = f->prim;
    struct lefnodo_t *pos = NULL;

    // Verifica duplicata e acha a posição
    while (aux) {
      if (aux->item == item) {
        free(nodo);
        return -1;
      }

      if (aux->prio <= prio)
        pos = aux;

      aux = aux->prox;
      }

    // Se todos os itens tiverem prioridade maior, então o novo item é o primeiro da fila
    if (!pos) {
      nodo->prox = f->prim;
      f->prim = nodo;
    }

    else {
      nodo->prox = pos->prox;
      pos->prox = nodo;
    }
  }

  f->num++;
  return f->num;
}

// Retira o primeiro item da fila e o devolve; o tipo e a prioridade
void *lef_retira (struct lef_t *f, int *tipo, int *prio)
{
  if (!f || !tipo || !prio || lef_tamanho(f) <= 0)
    return NULL;
  
  struct lefnodo_t *aux = f->prim;
  // Coloca o segundo da fila como primeiro e diminue o tamanho da fila
  f->prim = aux->prox;
  f->num--;

  *tipo = aux->tipo;
  *prio = aux->prio;
  void *item = aux->item;
  
  free(aux);
  
  return item;
}

// Informa o número de itens na fila.
int lef_tamanho (struct lef_t *f)
{
  if (!f)
    return -1;
  
  return (f->num < 0) ? -1 : f->num;
}

// Imprime o conteúdo da fila no formato "tipo prio tipo prio ..."
void lef_imprime (struct lef_t *f)
{
  if (!f)
    return;

  struct lefnodo_t *aux = f->prim;

  while (aux) {
    printf ("%d %d", aux->tipo, aux->prio);

    // Só da espaço se tiver prox item
    if (aux->prox)
      printf (" ");

    aux = aux->prox;
  }
}

// Recebe um herói H, um tipo de evento
// Se o evento for falso (0), retira todos os eventos relacionados a esse H
void lef_limpeza_heroi(struct lef_t *f, heroi_t *h, int tipo)
{
  if (!f || !h)
    return;

  struct lefnodo_t *ant = NULL;
  struct lefnodo_t *aux = f->prim;

  while (aux) {
    struct lefnodo_t *prox = aux->prox;
    evento_t *ev = aux->item;

    if (ev->campo1 == h && (!tipo || aux->tipo == tipo)) {
      if (ant)
        ant->prox = prox;
      else
        f->prim = prox;

      f->num--;
      destroi_evento(ev);
      free(aux);

      if (tipo)   // Se realmente for um tipo, quebra o laço após retirar só um
        break;
    } 
    
    else
      ant = aux;

    aux = prox;
  }
}