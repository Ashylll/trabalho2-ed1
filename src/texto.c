#include "texto.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define FFAMILY_PADRAO "sans-serif"
#define FWEIGHT_PADRAO "normal"
#define FSIZE_PADRAO 20
#define CONST_AREA 20.0

typedef struct stEstilo{
    char *fFamily;
    char *fWeight;
    int fSize;
} stEstilo;

typedef struct stTexto{
    int i;
    double x, y;
    char *corb, *corp, a, *txto;

    stEstilo estilo;
} stTexto;

TEXTO criar_texto(int i, double x, double y, const char* corb, const char* corp, char a, const char* txto){
    if (!corb || !corp || !txto) {
        fprintf(stderr, "Cores/texto inválidos\n");
        return NULL;
    }

    if (a != 'i' && a != 'm' && a != 'f') {
        fprintf(stderr, "Parâmetro 'a' inválido\n");
        return NULL;
    }
    
    stTexto *texto = malloc(sizeof(*texto));
    if (!texto) {
        fprintf(stderr, "Erro na alocação de memória\n");
        return NULL;
    }
    
    texto->i = i;
    texto->x = x;
    texto->y = y;
    texto->a = a;

    texto->estilo.fFamily = malloc(strlen(FFAMILY_PADRAO)+1);
     if (!texto->estilo.fFamily) {
        fprintf(stderr, "Erro na alocação de memória\n");
        free(texto);
        return NULL;
    }
    strcpy(texto->estilo.fFamily, FFAMILY_PADRAO);

    texto->estilo.fWeight = malloc(strlen(FWEIGHT_PADRAO)+1);
    if (!texto->estilo.fWeight){
        fprintf(stderr, "Erro na alocação de memória\n");
        free(texto->estilo.fFamily);
        free(texto);
        return NULL;
    }
    strcpy(texto->estilo.fWeight, FWEIGHT_PADRAO);

    texto->estilo.fSize = FSIZE_PADRAO;

    texto->corb = malloc(strlen(corb)+1);
    if (!texto->corb){
        fprintf(stderr, "Erro na alocação de memória\n");
        free(texto->estilo.fFamily);
        free(texto->estilo.fWeight);
        free(texto);
        return NULL;    
    }
    strcpy(texto->corb, corb);

    texto->corp = malloc(strlen(corp)+1);
    if (!texto->corp){
        fprintf(stderr, "Erro na alocação de memória\n");
        free(texto->corb);
        free(texto->estilo.fFamily);
        free(texto->estilo.fWeight);
        free(texto);
        return NULL;    
    }
    strcpy(texto->corp, corp);

    texto->txto = malloc(strlen(txto)+1);
    if (!texto->txto){
        fprintf(stderr, "Erro na alocação de memória\n");
        free(texto->corp);
        free(texto->corb);
        free(texto->estilo.fFamily);
        free(texto->estilo.fWeight);
        free(texto);
        return NULL;    
    }
    strcpy(texto->txto, txto);

    return texto;

}

double area_texto(TEXTO t){
    assert (t != NULL);

    stTexto *texto = (stTexto*)t;

    if (!texto->txto) return 0.0;

    double area = CONST_AREA * (double)(strlen(texto->txto));

    return area;
}

void destruir_texto(TEXTO *t){
    if (!t || !*t) return;
    stTexto *texto = (stTexto*)*t;

    free(texto->estilo.fFamily);
    free(texto->estilo.fWeight);
    free(texto->corb);
    free(texto->corp);
    free(texto->txto);

    free(texto);
    
    *t = NULL;
}

int getI_texto(TEXTO t){
    assert (t != NULL);
    stTexto *texto = (stTexto*)t;

    return texto->i;

}



double getX_texto(TEXTO t){
    assert (t != NULL);
    stTexto *texto = (stTexto*)t;

    return texto->x;

}


double getY_texto(TEXTO t){
    assert (t != NULL);
    stTexto *texto = (stTexto*)t;

    return texto->y;

}


const char* getCORB_texto(TEXTO t){
    assert (t != NULL);
    stTexto *texto = (stTexto*)t;

    return texto->corb;
}


const char* getCORP_texto(TEXTO t){
    assert (t != NULL);
    stTexto *texto = (stTexto*)t;

    return texto->corp;
}


char getA_texto(TEXTO t){
    assert (t != NULL);
    stTexto *texto = (stTexto*)t;

    return texto->a;
}


const char* getTXTO_texto(TEXTO t){
    assert (t != NULL);
    stTexto *texto = (stTexto*)t;

    return texto->txto;
}

bool setI_texto(TEXTO t, int i){
    if (!t) return false;

    stTexto *texto = (stTexto*)t;
    texto->i = i;

    return true;
}

bool setX_texto(TEXTO t, double x){
    if (!t) return false;

    stTexto *texto = (stTexto*)t;
    texto->x = x;

    return true;
}

bool setY_texto(TEXTO t, double y){
    if (!t) return false;

    stTexto *texto = (stTexto*)t;
    texto->y = y;

    return true;
}

bool setCORB_texto(TEXTO t, const char *corb){
    if (!t || !corb) return false;

    stTexto *texto = (stTexto*)t;

    if (texto->corb && strcmp(texto->corb, corb) == 0) return true;

    char *novo = malloc(strlen(corb)+1);
    if (!novo) return false;

    strcpy(novo, corb);
    free(texto->corb);
    texto->corb = novo;

    return true;
}

bool setCORP_texto(TEXTO t, const char *corp){
    if (!t || !corp) return false;

    stTexto *texto = (stTexto*)t;
    
    if (texto->corp && strcmp(texto->corp, corp) == 0) return true;

    char *novo = malloc(strlen(corp)+1);
    if (!novo) return false;

    strcpy(novo, corp);
    free(texto->corp);
    texto->corp = novo;

    return true;
}

bool setA_texto(TEXTO t, char a){
    if (!t) return false;
    if (a != 'i' && a != 'm' && a != 'f') return false;

    stTexto *texto = (stTexto*)t;
    texto->a = a;

    return true;
}

bool setTXTO_texto(TEXTO t, const char *txto){
    if (!t || !txto) return false;

    stTexto *texto = (stTexto*)t;

    if (texto->txto && strcmp(texto->txto, txto) == 0) return true;

    char *novo = malloc(strlen(txto)+1);
    if (!novo) return false;

    strcpy(novo, txto);
    free(texto->txto);
    texto->txto = novo;

    return true;
}

bool mudar_estilo(TEXTO t, const char *fFamily, const char *fWeight, int fSize){
    if (!t || !fFamily || !fWeight || fSize <= 0) return false;

    stTexto *texto = (stTexto*)t;

    if (texto->estilo.fFamily && texto->estilo.fWeight &&
        strcmp(texto->estilo.fFamily, fFamily) == 0 &&
        strcmp(texto->estilo.fWeight, fWeight) == 0 &&
        texto->estilo.fSize == fSize) {
        return true;
    }

    char *novaFamily = malloc(strlen(fFamily) + 1);
    if (!novaFamily) {
        fprintf(stderr, "Erro na alocação de memória\n");
        return false;
    }

    char *novoWeight = malloc(strlen(fWeight) + 1);
    if (!novoWeight) {
        free(novaFamily);
        fprintf(stderr, "Erro na alocação de memória\n");
        return false;
    }

    strcpy(novaFamily, fFamily);
    free(texto->estilo.fFamily);

    strcpy(novoWeight, fWeight);
    free(texto->estilo.fWeight);

    texto->estilo.fFamily = novaFamily;
    texto->estilo.fWeight = novoWeight;
    texto->estilo.fSize = fSize;

    return true;
}

const char* getFFamily_texto(TEXTO t){
    if (!t) return NULL;
    stTexto *texto = (stTexto*)t;

    return texto->estilo.fFamily; 
}

const char* getFWeight_texto(TEXTO t){
    if (!t) return "n";
    stTexto *texto = (stTexto*)t;

    return texto->estilo.fWeight;
}

int getFSize_texto(TEXTO t){
    if (!t) return 0;
    stTexto *texto = (stTexto*)t;

    return texto->estilo.fSize;
}