// programa principal do projeto "The Boys - 2024/2"
// Autor: xxxxx, GRR xxxxxx

// seus #includes vão aqui
#include <stdlib.h>
#include "mundo.h"
#include <time.h>

// seus #defines vão aqui

// minimize o uso de variáveis globais

// programa principal
int main ()
{
  srand((unsigned) time(NULL));
  
  mundo_t *w = inicializa_mundo();
  simulacao(w);
  w = destroi_mundo(w);

  return (0) ;
}