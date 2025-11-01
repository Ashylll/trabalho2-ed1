#include "circulo.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define PI 3.141592653589793

typedef struct stCirculo{
    int i;
    double x, y, r;
    char *corb, *corp;
} stCirculo;

CIRCULO criar_circulo(int i, double x, double y, double r, const char *corb, const char *corp){
    if(!corb || !corp || r <= 0){
        fprintf(stderr, "Parâmetros inválidos\n");
        return NULL;
    }
    stCirculo *circulo = malloc(sizeof(*circulo));
   if (!circulo){
    fprintf(stderr, "Erro na alocação de memória\n");
    return NULL;
   }

    circulo->i = i;
    circulo->x = x;
    circulo->y = y;
    circulo->r = r;
    
    circulo->corb = malloc(strlen(corb)+1);
    if (circulo->corb == NULL){
        fprintf(stderr, "Erro na alocação de memória\n");
        free(circulo);
        return NULL;
    }
    strcpy(circulo->corb, corb);

     circulo->corp = malloc(strlen(corp)+1);
    if (circulo->corp == NULL){
        fprintf(stderr, "Erro na alocação de memória\n");
        free(circulo->corb);
        free(circulo);
        return NULL;
    }
    strcpy(circulo->corp, corp);

    return circulo;
}

double area_circulo(CIRCULO c){
    assert (c != NULL);
    stCirculo *circulo = (stCirculo*)c;

    double area = PI * circulo->r * circulo->r;
    
    return area;
}

void destruir_circulo(CIRCULO *c){
    if (!c || !*c) return;
    stCirculo *circulo = (stCirculo*) *c;

    free(circulo->corb);
    free(circulo->corp);
    free(circulo);

    *c = NULL;
}

int getI_circulo(CIRCULO c){
    assert(c != NULL);
    stCirculo *circulo = (stCirculo*)c;

    return circulo->i;
}

double getX_circulo(CIRCULO c){
    assert(c != NULL);
    stCirculo *circulo = (stCirculo*)c;

    return circulo->x;
}

double getY_circulo(CIRCULO c){
    assert(c != NULL);
    stCirculo *circulo = (stCirculo*)c;

    return circulo->y;
}

double getR_circulo(CIRCULO c){
    assert(c != NULL);
    stCirculo *circulo = (stCirculo*)c;

    return circulo->r;
}

const char* getCORB_circulo(CIRCULO c){
    assert(c != NULL);
    stCirculo *circulo = (stCirculo*)c;

    return circulo->corb;
}

const char* getCORP_circulo(CIRCULO c){
    assert(c != NULL);
    stCirculo *circulo = (stCirculo*)c;

    return circulo->corp;
}

bool setI_circulo(CIRCULO c, int i){
    if (!c) return false;

    stCirculo *circulo = (stCirculo*)c; 
    circulo->i = i;
    return true;
}

bool setX_circulo(CIRCULO c, double x){
    if (!c) return false;

   stCirculo *circulo = (stCirculo*)c; 
    circulo->x = x;

    return true;
}

bool setY_circulo(CIRCULO c, double y){
    if (!c) return false;

    stCirculo *circulo = (stCirculo*)c; 
    circulo->y = y;

    return true;
}

bool setR_circulo(CIRCULO c, double r){
    if (!c || r <= 0) return false;

    stCirculo *circulo = (stCirculo*)c; 
    circulo->r = r;

    return true;
}

bool setCORB_circulo(CIRCULO c, const char* corb){
    if (!c || !corb) return false;

    stCirculo *circulo = (stCirculo*)c; 

    if (circulo->corb && strcmp(circulo->corb, corb) == 0) return true;
    
    char* novo = malloc(strlen(corb)+1);
    if(!novo) return false;

    strcpy(novo, corb);
    free(circulo->corb);
    circulo->corb = novo;

    return true;
}

bool setCORP_circulo(CIRCULO c, const char* corp){
    if (!c || !corp) return false;

    stCirculo *circulo = (stCirculo*)c; 
    
    if (circulo->corp && strcmp(circulo->corp, corp) == 0) return true;
    
    char* novo = malloc(strlen(corp)+1);
    if(!novo) return false;

    strcpy(novo, corp);
    free(circulo->corp);
    circulo->corp = novo;

    return true;
}