#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>

/*
    lista.h
    Módulo responsável pela implementação da lista ligada (armazaena formas)

*/

typedef void* LISTA;
typedef void* FORMA;

/// @brief cria uma lista vazia
/// @return ponteiro para lista vazia
LISTA criar_lista(void);

/// @brief verifica se a lista está vazia
/// @param l lista
/// @return true se vazia; false se não
bool vazia_lista(LISTA l);

/// @brief adiciona uma forma à lista
/// @param l lista
///  @param f forma
/// @return true se a operação foi bem sucedida; false se não
void adicionar_lista(LISTA l, FORMA f);

/// @brief remove uma forma da lista
/// @param l lista
/// @param f forma a ser removida
/// @return forma removida
void* remover_lista(LISTA l, FORMA f);

/// @brief busca uma forma por id
/// @param l lista
/// @param id id da forma
/// @return forma se encontrou; NULL se não
void* buscar_lista(LISTA l, const int id);


#endif
