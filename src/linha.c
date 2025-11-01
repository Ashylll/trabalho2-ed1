#include "linha.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

typedef struct stLinha {
    int i;
    double x1, y1, x2, y2;
    char *cor;
} stLinha;

LINHA criar_linha(int i, double x1, double y1, double x2, double y2, const char* cor){
    if (!cor) {
        fprintf(stderr, "Cor inválida\n");
        return NULL;
    }

    stLinha *linha = malloc(sizeof(*linha));
    if (!linha){
    fprintf(stderr, "Erro na alocação de memória\n");
    return NULL;
   }

    linha->i = i;
    linha->x1 = x1;
    linha->y1 = y1;
    linha->x2 = x2;
    linha->y2 = y2;

    linha->cor = malloc(strlen(cor)+1);
    if (linha->cor == NULL){
        fprintf(stderr, "Erro na alocação de memória\n");
        free(linha);
        return NULL;
    }
    strcpy(linha->cor, cor);

    return linha;
}

double comprimento_linha(LINHA l){
    assert (l != NULL);
    stLinha *linha = (stLinha*)l;

    double deltaX = linha->x2 - linha->x1;
    double deltaY = linha->y2 - linha->y1;
    double comprimento = sqrt(deltaX*deltaX + deltaY*deltaY);

    return comprimento;
}

double area_linha(LINHA l){
    assert (l != NULL);
    double area = 2 * comprimento_linha(l);

    return area;
}

void destruir_linha(LINHA *l){
    if (!l || !*l) return;
    stLinha *linha = (stLinha*)*l;
    
    free(linha->cor);
    free(linha);
    
    *l = NULL;
}

// Funções get

int getI_linha(LINHA l){
    assert (l != NULL);
    stLinha *linha = (stLinha*)l;

    return linha->i;
}

double getX1_linha(LINHA l){
    assert (l != NULL);
    stLinha *linha = (stLinha*)l;

    return linha->x1;
}

double getY1_linha(LINHA l){
    assert (l != NULL);
    stLinha *linha = (stLinha*)l;

    return linha->y1;
}

double getX2_linha(LINHA l){
    assert (l != NULL);
    stLinha *linha = (stLinha*)l;

    return linha->x2;
}

double getY2_linha(LINHA l){
    assert (l != NULL);
    stLinha *linha = (stLinha*)l;

    return linha->y2;
}

const char* getCOR_linha(LINHA l){
    assert (l != NULL);
    stLinha *linha = (stLinha*)l;

    return linha->cor;
}


// Funções set

bool setI_linha(LINHA l, int i){
    if (!l) return false;

    stLinha *linha = (stLinha*)l; 
    linha->i = i;

    return true;
}

bool setX1_linha(LINHA l, double x1){
    if (!l) return false;

    stLinha *linha = (stLinha*)l; 
    linha->x1 = x1;
    
    return true;
}

bool setY1_linha(LINHA l, double y1){
    if (!l) return false;

    stLinha *linha = (stLinha*)l; 
    linha->y1 = y1;

    return true;
}

bool setX2_linha(LINHA l, double x2){
    if (!l) return false;

    stLinha *linha = (stLinha*)l; 
    linha->x2 = x2;

    return true;
}

bool setY2_linha(LINHA l, double y2){
    if (!l) return false;

    stLinha *linha = (stLinha*)l; 
    linha->y2 = y2;

    return true;
}

bool setCOR_linha(LINHA l, const char* cor){
    if (!l || !cor) return false;
    stLinha *linha = (stLinha*)l;

    if (linha->cor && strcmp(linha->cor, cor) == 0) return true;

    char *novo = malloc(strlen(cor)+1);
    if (!novo) return false;

    strcpy(novo, cor);
    free(linha->cor);
    linha->cor = novo;

    return true;
}