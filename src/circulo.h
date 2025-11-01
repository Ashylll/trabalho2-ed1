#ifndef CIRCULO_H
#define CIRCULO_H

#include <stdbool.h>

/*
    circulo.h
    Módulo responsável pela criação e gerenciamento de círculos
    
    Convenções do projeto:
   - Sistema de coordenadas com (0,0) no canto superior esquerdo;
   - x cresce para a direita; y cresce para baixo;

*/

typedef void* CIRCULO;

/// @brief cria um círculo com identificador, cor de borda e de preenchimento e coordenadas
/// @param i identificador numérico do círculo
/// @param x coordenada do eixo x
/// @param y coordenada do eixo y
/// @param r raio
/// @param corb cor da borda (string)
/// @param corp cor de preenchimento (string)
/// @return ponteiro para o círculo; NULL se parâmetros inválidos ou falha de memória
CIRCULO criar_circulo(int i, double x, double y, double r, const char* corb, const char* corp);

/// @brief retorna a área do círculo    
/// @param c círculo
/// @pre c != NULL
/// @return área
double area_circulo(CIRCULO c);

/// @brief libera o círculo
/// @param c ponteiro para o handle do círculo
void destruir_circulo(CIRCULO *c);

/* === Operações get === */

/// @brief retorna o identificador do círculo
/// @param c círculo
/// @pre c != NULL
/// @return identificador 
int getI_circulo(CIRCULO c);

/// @brief retorna a coordenada x do círculo 
/// @param c círculo    
/// @pre c != NULL
/// @return coordenada x 
double getX_circulo(CIRCULO c);

/// @brief retorna a coordenada y do círculo 
/// @param c círculo    
/// @pre c != NULL
/// @return coordenada y 
double getY_circulo(CIRCULO c);

/// @brief retorna o raio do círculo 
/// @param c círculo    
/// @pre c != NULL
/// @return raio 
double getR_circulo(CIRCULO c);

/// @brief retorna a cor da borda do círculo
/// @param c círculo
/// @pre c != NULL
/// @return ponteiro para string com a cor da borda (apenas leitura)
const char* getCORB_circulo(CIRCULO c);

/// @brief retorna a cor de preenchimento do círculo
/// @param c círculo
/// @pre c != NULL
/// @return ponteiro para string com a cor de preenchimento (apenas leitura)
const char* getCORP_circulo(CIRCULO c);

/* ===================== */


/* === Operações set === */

/// @brief atribui um identificador ao círculo
/// @param c círculo
/// @param i identificador
/// @return true se a operação foi bem sucedida; false se c == NULL
bool setI_circulo(CIRCULO c, int i);

/// @brief atribui uma coordenada x ao círculo
/// @param c círculo
/// @param x coordenada x
/// @return true se a operação foi bem sucedida; false se c == NULL
bool setX_circulo(CIRCULO c, double x);

/// @brief atribui uma coordenada y ao círculo
/// @param c círculo
/// @param y coordenada y
/// @return true se a operação foi bem sucedida; false se c == NULL
bool setY_circulo(CIRCULO c, double y);

/// @brief atribui um raio ao círculo
/// @param c círculo
/// @param r raio
/// @return true se a operação foi bem sucedida; false se c == NULL || r <= 0
bool setR_circulo(CIRCULO c, double r);

/// @brief atribui uma cor de borda ao círculo
/// @param c círculo
/// @param corb cor de borda 
/// @return true se a operação foi bem sucedida; false se c == NULL || corb == NULL
bool setCORB_circulo(CIRCULO c, const char* corb);

/// @brief atribui uma cor de preenchimento ao círculo
/// @param c círculo
/// @param corp cor de preenchimento
/// @return true se a operação foi bem sucedida; false se c == NULL || corp == NULL
bool setCORP_circulo(CIRCULO c, const char* corp);

/* ===================== */

#endif
