#ifndef LINHA_H
#define LINHA_H

#include <stdbool.h>

/*
    linha.h
    Módulo responsável pela criação e gerenciamento de linhas

*/

typedef void* LINHA;

/// @brief cria uma linha com identificador e cor
/// @param i identificador 'i'
/// @param x1 coordenada x da extremidade 1 (x1)
/// @param y1 coordenada y da extremidade 1 (y1)
/// @param x2 coordenada x da extremidade 2 (x2)
/// @param y2 coordenada y da extremidade 2 (y2)
/// @param cor cor da linha
/// @return retorna ponteiro para a linha
LINHA criar_linha(int i, double x1, double y1, double x2, double y2, const char* cor);

/// @brief calcula o comprimento da linha
/// @param l linha
/// @pre l != NULL
/// @return comprimento da linha
double comprimento_linha(LINHA l);

/// @brief calcula a área da linha
/// @param l linha
/// @pre l != NULL
/// @return área da linha
double area_linha(LINHA l);

/// @brief libera a linha
/// @param l ponteiro para o handle da linha
void destruir_linha(LINHA *l);

/* === Operações get === */

/// @brief retorna o identificador da linha
/// @param l linha
/// @pre l != NULL
/// @return identificador 
int getI_linha(LINHA l);

/// @brief retorna a coordenada x1 da linha
/// @param l linha 
/// @pre l != NULL
/// @return coordenada x1 
double getX1_linha(LINHA l);

/// @brief retorna a coordenada y1 da linha
/// @param l linha   
/// @pre l != NULL
/// @return coordenada y1 
double getY1_linha(LINHA l);

/// @brief retorna a coordenada x2 da linha
/// @param l linha
/// @pre l != NULL
/// @return coordenada x2
double getX2_linha(LINHA l);

/// @brief retorna a coordenada y2 da linha
/// @param l linha
/// @pre l != NULL
/// @return coordenada y2
double getY2_linha(LINHA l);

/// @brief retorna a cor da linha
/// @param l linha
/// @pre l != NULL
/// @return ponteiro para string com a cor da linha (apenas leitura)
const char* getCOR_linha(LINHA l);

/* ===================== */


/* === Operações set === */

/// @brief atribui um identificador à linha
/// @param l linha
/// @param i identificador
/// @return true se a operação foi bem sucedida; false se não
bool setI_linha(LINHA l, int i);

/// @brief atribui uma coordenada x1 à linha
/// @param l linha 
/// @param x1 coordenada x1
/// @return true se a operação foi bem sucedida; false se não
bool setX1_linha(LINHA l, double x1);

/// @brief atribui ua coordenada y1 à linha
/// @param l linha   
/// @param y1 coordenada y1 
/// @return true se a operação foi bem sucedida; false se não 
bool setY1_linha(LINHA l, double y1);

/// @brief atribui uma coordenada x2 à linha
/// @param l linha
/// @param x2 coordenada x2
/// @return true se a operação foi bem sucedida; false se não
bool setX2_linha(LINHA l, double x2);

/// @brief atribui uma coordenada y2 à linha
/// @param l linha
/// @param y2 coordenada y2 
/// @return true se a operação foi bem sucedida; false se não
bool setY2_linha(LINHA l, double y2);

/// @brief atribui uma cor à linha
/// @param l linha
/// @param cor cor
/// @return true se a operação foi bem sucedida; false se não
bool setCOR_linha(LINHA l, const char* cor);

/* ===================== */

#endif