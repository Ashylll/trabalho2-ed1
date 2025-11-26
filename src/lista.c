#include "lista.h"
#include "forma.h"

#include <stdlib.h>
#include <assert.h>

typedef struct elemento {
    FORMA forma;
    struct elemento *prox;
} ELEMENTO;

typedef ELEMENTO *PONT;

typedef struct stLista {
    PONT inicio;
} stLista;

LISTA criar_lista(void) {
    stLista *lista = malloc(sizeof(*lista));
    assert(lista);

    lista->inicio = NULL;

    return lista;
}

bool vazia_lista(LISTA l) {
    assert(l);
    const stLista *lista = (stLista*)l;

    return  (lista->inicio == NULL) ;
}

void adicionar_lista(LISTA l, FORMA f) {
    assert (l && f);
    stLista *lista = l;

    const PONT novo = malloc(sizeof(ELEMENTO));
    assert (novo);

    novo->forma = f;
    novo->prox = lista->inicio;
    lista->inicio = novo;
}

void* remover_lista(LISTA l, FORMA f) {
    assert(l);
    stLista *lista = l;

    PONT anterior = NULL;
    PONT p = lista->inicio;

    while (p && p->forma != f) {
        anterior = p;
        p = p->prox;
    }

    if (!p) return NULL;

    if (!anterior) lista->inicio = p->prox;
    else anterior->prox = p->prox;
    FORMA removida = p->forma;
    free(p);

    return removida;
}

void* buscar_lista(LISTA l, const int id) {
    assert (l);
    const stLista *lista = l;

    PONT busca = lista->inicio;
    while (busca) {
        if (getI_forma(busca->forma) == id) {
            return busca->forma;
        }
        busca = busca->prox;
    }

    return NULL;
}
