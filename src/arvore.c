#include "arvore.h"


#include <stdlib.h>
#include <assert.h>

typedef struct no {
    void *chave;

    struct no *esq;
    struct no *dir;
} NO;

typedef struct {
    NO *raiz;
    int (*cmp)(const void* A, const void* B);
} stArvore;

static NO** encontrar_posicao(stArvore *arvore, const void *chave, bool *achou) {
    NO **p = &arvore->raiz;

    while (*p) {
        const int r = arvore->cmp(chave, (*p)->chave);

        if (r == 0) {
            *achou = true;
            return p;
        }

        if (r > 0)
            p = &(*p)->esq;
        else
            p = &(*p)->dir;
    }

    *achou = false;
    return p;
}

ARVORE criar_arvore(int (*cmp)(const void*A, const void* B)) {
    stArvore *arvore = malloc(sizeof(stArvore));

    arvore->raiz = NULL;
    arvore->cmp = cmp;

    return (ARVORE)arvore;
}

bool inserir_arvore(ARVORE a, void *chave) {
    assert(a && chave);

    stArvore *arvore = a;
    bool achou = false;

    NO **p = encontrar_posicao(arvore, chave, &achou);
    if (achou) return false;

    NO *novo = malloc(sizeof(NO));
    if (!novo) return false;

    novo->chave = chave;
    novo->esq = novo->dir = NULL;

    *p = novo;

    return true;
}

void* remover_arvore(ARVORE a, const void *chave) {
    assert (a && chave);

    stArvore *arvore = a;
    bool achou = false;

    NO **p = encontrar_posicao(arvore, chave, &achou);
    if (!achou) return NULL;

    (*p)->chave;

}

void* buscar_arvore(ARVORE a, void *chave) {
    assert (a && chave);

    stArvore *arvore = a;
    bool achou = false;

    NO **p = encontrar_posicao(arvore, chave, &achou);
    if (!achou) return NULL;

    NO *alvo = *p;
    void *item = alvo->chave;

    return (*p)->chave;;
}

