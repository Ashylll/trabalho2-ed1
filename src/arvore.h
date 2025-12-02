#ifndef ARVORE_H
#define ARVORE_H

#include <stdbool.h>

/*
    lista.h
    Módulo responsável pela implementação da árvore binária de busca

*/

typedef void* ARVORE;

/// @brief cria uma árvore com raiz vazia
/// @param cmp função coringa para os algoritmos de ordenação (sort/merge/insertion)
/// @return ponteiro para a árvore
ARVORE criar_arvore(int (*cmp)(const void* A, const void* B));

/// @brief insere um elemento à arvore
/// @param a ávore
/// @param chave elemento a ser inserido
/// @return true se inseriu; false se já existia
bool inserir_arvore(ARVORE a, void *chave);

/// @brief remove um nó da árvore
/// @param a árvore
/// @param chave elemento a ser retirado
/// @return elemento removido
void* remover_arvore(ARVORE a, const void *chave);

/// @brief busca um elemento na árvore
/// @param a árvore
/// @param chave elemento a ser buscado
/// @return elemento se encontrado ou NULL
void* buscar_arvore(ARVORE a, void *chave);




#endif
