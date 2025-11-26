#ifndef ANTEPARO_H
#define ANTEPARO_H

#include "lista.h"

/*
    lista.h
    Módulo responsável pela implementação da lista ligada (armazaena formas)

*/

typedef void* ANTEPARO;

/// @brief gera um id único para o anteparo
/// @return número inteiro e negativo, decrementando a partir do -1
int gerar_id_anteparo(void);

/// @brief transforma formas com id dentro do intervalo [i,j] em anteparos
/// @param l_formas lista de formas
/// @param l_anteparos lista de anteparos
/// @param i id menor
/// @param j id maior
/// @param v_h 'v' ou 'h' para transformar os círculos em segmentos verticais ou horizontais
void comando_anteparo(LISTA l_formas, LISTA l_anteparos, int i, const int j, const char v_h);

void comando_destruicao(const double x, const double y, const char sfx);

void comando_pintura(const double x, const double y, const char *cor, const char sfx);

void comando_clonagem(const double x, const double y, const double dx, const double dy, const char sfx);












#endif
