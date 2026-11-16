// programa principal do projeto "The Boys - 2026/2"
// Autor: Guilherme Cubas Alves, GRR 20262712

// seus #includes vão aqui
#include <stdlib.h>
#include <time.h>
#include "mundo.h"

int main ()
{
  srand(time(NULL));            // Inicializa semente random
  
  mundo_t *w = mundo_ini();
  simulacao(w);
  w = mundo_destroi(w);

  return (0) ;
}