#ifndef EVENTOS
#define EVENTOS

#define AGORA 0
#define CHEGA 1
#define ESPERA 2
#define DESISTE 3
#define AVISA 4
#define ENTRA 5
#define SAI 6
#define VIAJA 7
#define MORRE 8
#define MISSAO 9
#define INVADE 10
#define RECUPERA_HEROI 11
#define RECONSTROI_BASE 12
#define FIM 13

// Declaração opaca pq tava dando problema nos include
typedef struct mundo mundo_t;

// EVENTO
typedef struct
{
    void *campo1;       // Ponteiro genérico para campo genérico, normalmente herói
    void *campo2;       //---, normalmente base
} evento_t;

// Agenda um evento de qualquer tipo com campo1 e campo2, para o horario atual+time
void agenda_evento (mundo_t *w, void *campo1, void *campo2, int tipo, int time);

// Libera a memoria alocada de um evento
evento_t *destroi_evento (evento_t *ev);

// Evento do heroi H chegando na base B
void chega (mundo_t *w, evento_t *ev);

// Evento do heroi H entrando na fila de espera de B
void espera (mundo_t *w, evento_t *ev);

// Evento do heroi H desistindo de entrar na base B e procurando outra
void desiste (mundo_t *w, evento_t *ev);

// Evento do porteiro da base B tratando da fila de espera
void avisa (mundo_t *w, evento_t *ev);

// Evento do heroi H entrando na base B
void entra (mundo_t *w, evento_t *ev);

// Evento do heroi H saindo da base B e escolhe outra pra viajar
void sai (mundo_t *w, evento_t *ev);

// Evento do heroi H viajando ate uma base D
void viaja (mundo_t *w, evento_t *ev);

// Evento de morte de um heroi H
void morre (mundo_t *w, evento_t *ev);

// Evento de uma missao M
void missao (mundo_t *w, evento_t *ev);

// Evento de uma invasão INV acontecendo em uma base B
void invade (mundo_t *w, evento_t *ev);

// Evento de um heroi H ficando ativo pos invasão
void recupera_heroi (evento_t *ev);

// Evento de um base B ficando ativa pos invasão
void reconstroi_base (evento_t *ev);

// Encerra a simulação e apresenta suas estatísticas
void fim (mundo_t *w, evento_t *ev);

#endif