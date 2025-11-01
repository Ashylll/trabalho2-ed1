#ifndef FORMA_H
#define FORMA_H

#include <stdbool.h>
#include <stddef.h>

/*
    forma.h
    Módulo responsável por conciliar e unificar operações sobre as formas do projeto
    
    Convenção de tipos:
      'c' = círculo
      'r' = retângulo
      't' = texto
      'l' = linha

*/

typedef void* FORMA;

/// @brief registra um id lido no arquivo .geo, atualizando o maior id global
/// @param id identificador da forma
void registrar_id(int id);

/// @brief gera um novo id único para clones
/// @return id novo, incrementando o maior id atual
int gerar_id_clone(void);

/// @brief cria uma forma
/// @param tipo tipo da forma (círculo, retângulo, texto ou linha)
/// @param handle handle para a forma (ponteiro para a forma)
/// @return ponteiro para a forma
FORMA criar_forma(char tipo, void* handle);

/// @brief libera a forma
/// @param f ponteiro para handle FORMA
void destruir_forma(FORMA *f);

/// @brief retorna a área da forma
/// @param f forma
/// @pre f!= NULL
/// @return área
double area_forma(FORMA f);

/* === Operações get === */

/// @brief retorna o identificador da forma
/// @param f forma
/// @pre f != NULL
/// @return identificador
int getI_forma(FORMA f);

/// @brief retorna o tipo da forma
/// @param f forma
/// @pre f != NULL
/// @return caractere para o tipo ('c', 'r', 't', 'l')
char getTipo_forma(FORMA f);

/// @brief retorna o handle da forma
/// @param f forma
/// @return handle
void* getHandle_forma(FORMA f);

/* ===================== */


/* === Operações set === */

/// @brief lê as coordenadas (x,y) da forma
/// @param f forma
/// @param x ponteira para saída da coordenada x
/// @param y ponteira para saída da coordenada y
/// @return true se a operação foi bem sucedida; false se f == NULL
bool getXY_forma(FORMA f, double* x, double* y);

/// @brief atribui coordenadas (x,y) da forma
/// @param f forma
/// @param x coordenada x
/// @param y coordenada y
/// @return true se a operação foi bem sucedida; false se f == NULL
bool setXY_forma(FORMA f, double x, double y);

/// @brief desloca a forma nas coordenadas (x,y)
/// @param f forma
/// @param dx deslocamento no eixo x
/// @param dy deslocamento no eixo y
/// @return true se a operação foi bem sucedida; false se f == NULL
bool deslocar_forma(FORMA f, double dx, double dy);

/* ===================== */


/* === Operações relacionadas à sobreposição === */

/// @brief verifica se há sobreposição entre duas formas
/// @details Faz uso de funções auxiliares para analisar cada combinação de tipos e trata a simetria de entrada
/// @param a forma 1
/// @param b forma 2
/// @return true se há sobreposição; false se não há || ponteiros nulos
bool sobrepoe_formas(FORMA a, FORMA b);

/// @brief cria uma cópia da forma
/// @param f forma original
/// @pre f != NULL
/// @return nova forma clonada; NULL se erro ou tipo desconhecido
FORMA clonar_forma(FORMA f);

/// @brief Cor da borda de J recebe a cor de preenchimento de I.
/// @param i forma 1 (nada muda nela)
/// @param j forma 2 (a ser afetada)
/// @pre i != NULL && j != NULL
void trocar_cores(FORMA i, FORMA j);

/// @brief Inverte as cores de preenchimento e de borda da forma. Preenchimento -> borda, borda -> preenchimento; caso linha: cor -> cor complementar
/// @param f forma
void inverter_cores(FORMA f);

/// @brief converte uma cor (string) em hexadecimal para nome
/// @param hex cor como hexadecimal
/// @return cor como nome
const char* hex_para_nome(const char *hex);

/// @brief converte uma cor (string) em nome para hexadecimal
/// @param cor como nome
/// @return cor como hexadecimal
const char* nome_para_hex(const char *cor);

/* ============================================= */

#endif