// programa principal do projeto "The Boys - 2024/2"
// Autor: xxxxx, GRR xxxxxx

// seus #includes vão aqui
#include "mundo.h"
#include "simulacao.h"

// seus #defines vão aqui

// minimize o uso de variáveis globais

// programa principal
int main ()
{
  mundo_t *w = inicializa_mundo();

  simulacao(w);
  // executar o laço de simulação

  destroi_mundo(w);
  // destruir o mundo

  return (0) ;
}