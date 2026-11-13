// programa principal do projeto "The Boys - 2024/2"
// Autor: xxxxx, GRR xxxxxx

// seus #includes vão aqui
#include "mundo.h"

// seus #defines vão aqui

// minimize o uso de variáveis globais

// programa principal
int main ()
{
  mundo_t *w = inicializa_mundo();
  simulacao(w);
  destroi_mundo(w);

  return (0) ;
}