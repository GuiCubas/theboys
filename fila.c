#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fila.h"

// Cria uma fila vazia.
struct fila_t *fila_cria ()
{
  struct fila_t *fila = malloc (sizeof(struct fila_t));

  if (!fila)
    return NULL;
  memset(fila, 0, sizeof(struct fila_t));
  return fila;
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
struct fila_t *fila_destroi (struct fila_t *f)
{
  if (!f)
    return NULL;

  struct fila_nodo_t *aux = f->prim;
  struct fila_nodo_t *prox;

  // Da free em cada nodo
  while (aux) {
    prox = aux->prox;
    free(aux);
    aux = prox;
  }

  free(f);
  return NULL;
}

// Insere o item no final da fila.
int fila_insere (struct fila_t *f, int item)
{
  if (!f || !item)
    return -1;

  // Verifica duplicata
  struct fila_nodo_t *aux = f->prim;
  while (aux) {
    if (aux->item == item)
      return -1;
    aux = aux->prox;
  }

  // Inicializa o novo nodo
  struct fila_nodo_t *nodo = malloc (sizeof(struct fila_nodo_t));
  if (!nodo)
    return -1;

  nodo->item = item;
  nodo->prox = NULL;

  // Se a fila estiver vazia, o nodo novo é o primeiro
  if (!f->prim)
    f->prim = nodo;
  // Se não, vai dps do último
  else
    f->fim->prox = nodo;

  f->fim = nodo;
  f->num++;
  return f->num;
}

// Retira o primeiro item da fila e o devolve
int fila_retira (struct fila_t *f)
{
  if (!f || fila_tamanho(f) <= 0)
    return -1;

  struct fila_nodo_t *aux = f->prim;

  // Coloca o segundo da fila como primeiro e diminui o tamanho da fila
  f->prim = aux->prox;
  f->num--;

  // Se a fila ficou vazia, o ultimo tambem deixa de existir
  if (!f->prim)
    f->fim = NULL;

  int item = aux->item;

  free(aux);
  return item;
}

// Informa o número de itens na fila.
int fila_tamanho (struct fila_t *f)
{
  if (!f)
    return -1;
  return (f->num < 0) ? -1 : f->num;
}

// Imprime o conteúdo da fila.
void fila_imprime (struct fila_t *f)
{
  if (!f)
    return;

  struct fila_nodo_t *aux = f->prim;

  while (aux) {
    printf ("%d", aux->item);

    // Só da espaço se tiver prox item
    if (aux->prox)
      printf (" ");

    aux = aux->prox;
  }
}