#include "forma.h"
#include "retangulo.h"
#include "circulo.h"    
#include "texto.h"      
#include "linha.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stddef.h>

static char* str_dup_c99(const char *src){
    if (!src) return NULL;
    size_t len = strlen(src) + 1;
    char *dup = malloc(len);
    if (!dup) return NULL;
    memcpy(dup, src, len);
    return dup;
}

typedef struct stForma{
    char tipo;
    void* handle;
} stForma;

FORMA criar_forma(char tipo, void* handle){
    if (!handle || (tipo != 'c' && tipo != 'r' && tipo != 't' && tipo != 'l')) return NULL;

    stForma* forma = malloc(sizeof(stForma));
    if (!forma) return NULL;

    forma->tipo = tipo;
    forma->handle = handle;

    return forma;
}

static int maior_id = 0;

void registrar_id(int id){
    if (id > maior_id) maior_id = id;
}

int gerar_id_clone(void){
    return ++maior_id;
}

void destruir_forma(FORMA *f){
    if (!f || !*f) return;
    stForma *forma = (stForma*)*f;

    switch (forma->tipo){
        case 'c': destruir_circulo((CIRCULO*)&forma->handle); break;
        case 'r': destruir_retangulo((RETANGULO*)&forma->handle); break;
        case 't': destruir_texto((TEXTO*)&forma->handle); break;
        case 'l': destruir_linha((LINHA*)&forma->handle); break;
    }
    
    free(forma);
    *f = NULL;
}

double area_forma(FORMA f){
    assert(f != NULL);
    stForma *forma = (stForma*)f;

    switch (forma->tipo){
        case 'c': return area_circulo(forma->handle);
        case 'r': return area_retangulo(forma->handle);
        case 't': return area_texto(forma->handle);
        case 'l': return area_linha(forma->handle);
        default: return 0.0;
    }
}

int getI_forma(FORMA f){
    assert(f != NULL);

    stForma *forma = (stForma*)f;

    if(!forma->handle) return -1;

    switch (forma->tipo){
        case 'c': return getI_circulo(forma->handle);
        case 'r': return getI_retangulo(forma->handle);
        case 't': return getI_texto(forma->handle);
        case 'l': return getI_linha(forma->handle); 
        default: return - 1;
    }
}

char getTipo_forma(FORMA f){
    assert(f != NULL);
    stForma* forma = (stForma*)f;

    return forma->tipo;
}

void* getHandle_forma(FORMA f){
    if (!f) return NULL;
    stForma* forma = (stForma*)f;

    return forma->handle;
}

bool getXY_forma(FORMA f, double *x, double *y){
    if (!f || !x || !y) return false;

    stForma* forma = (stForma*)f;

    switch (forma->tipo){
        case 'c': *x = getX_circulo(forma->handle); *y = getY_circulo(forma->handle); break;
        case 'r': *x = getX_retangulo(forma->handle); *y = getY_retangulo(forma->handle); break;
        case 't': *x = getX_texto(forma->handle); *y = getY_texto(forma->handle); break;
        case 'l': *x = getX1_linha(forma->handle); *y = getY1_linha(forma->handle); break;
        default: return false;
    }

    return true;
}

bool setXY_forma(FORMA f, double x, double y){
    if (!f) return false;

    stForma* forma = (stForma*)f;

    switch (forma->tipo){
        case 'c': return setX_circulo(forma->handle, x) && setY_circulo(forma->handle, y);
        case 'r': return setX_retangulo(forma->handle, x) && setY_retangulo(forma->handle, y);
        case 't': return setX_texto(forma->handle, x) && setY_texto(forma->handle, y);
        case 'l': {
                double x1 = getX1_linha(forma->handle);
                double y1 = getY1_linha(forma->handle);
                double x2 = getX2_linha(forma->handle);
                double y2 = getY2_linha(forma->handle);
                double dx = x - x1;
                double dy = y - y1;
                return setX1_linha(forma->handle, x) && setY1_linha(forma->handle, y) &&
                setX2_linha(forma->handle, x2 + dx) && setY2_linha(forma->handle, y2 + dy);
        }
        default: return false;
    }
}

bool deslocar_forma(FORMA f, double dx, double dy){
    if (!f) return false;

    double x, y;

    if (!getXY_forma(f, &x, &y)) return false;

    return setXY_forma(f, x + dx, y + dy);
}

double dist_ponto_segmento(double px, double py, double x1, double y1, double x2, double y2){
    // Define a equação geral da reta (Ax + By + C = 0) a partir dos pontos (x1,y1) e (x2,y2)
    // vetor direcional da reta -> (A,B) = (x2 - x1, y2 - y1)
    // vetor normal (projeção ortogonal) -> (A,B) = (y1 - y2, x2 - x1)
    
    double A = y1 - y2;
    double B = x2 - x1;
    double C = x1*y2 - x2*y1;

    double denom = sqrt(A*A + B*B);
    if (denom < 1e-18)  return hypot(px - x1, py - y1); // Segmento degenerado tratado como ponto
    
    // Distância do ponto à reta -> |Apx + Bpy + C| / sqrt(A² + B²)
    double dist_reta = fabs(A*px + B*py + C) / denom; 

    // Projeção escalar normalizada -> t = w . v / ||v||
    double vx = x2 - x1, vy = y2 - y1;
    double t = ((px - x1)*vx + (py - y1)*vy) / (vx*vx + vy*vy);

    // Se t ∈ [0,1], a menor distância é perpendicular à reta
    if (t >= 0.0 && t <= 1.0) return dist_reta; 

    // Projeção fora -> checa distância até as extremidades
    double d1 = hypot(px - x1, py - y1);
    double d2 = hypot(px - x2, py - y2);

    return fmin(d1, d2);
}

static inline bool ponto_no_retangulo(double x, double y, double xR, double yR, double w, double h){
    const double eps = 1e-9;
    return (xR - eps <= x && x <= xR + w + eps) && (yR - eps <= y && y <= yR + h + eps);
}

static bool seg_intersect(double x1, double y1,double x2, double y2, double x3, double y3, double x4, double y4){
    // Segmento 1 -> P(t) = P1 + t(P2 - P1)
    // Segmento 2 -> P(u) = P3 + u(P4 - P3)
    // Instersecção -> P(t) = P(u), sistema com determinante D (regra de Crammer)
    double D = (x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4);
    
    const double eps = 1e-9;
    if (fabs(D) < eps) {
        double A = y1 - y2;
        double B = x2 - x1;
        double C = x1*y2 - x2*y1;

        if (fabs(A*x3 + B*y3 + C) > eps) return false; // Verifica se são paralelas

        // Verifica se intervalos se tocam (colinearidade)
        bool overlapX = fmax(fmin(x1, x2), fmin(x3, x4)) <= fmin(fmax(x1, x2), fmax(x3, x4)) + eps;
        bool overlapY = fmax(fmin(y1, y2), fmin(y3, y4)) <= fmin(fmax(y1, y2), fmax(y3, y4)) + eps;

        return overlapX && overlapY;
    }

    // t = Dt/D; u = Du/D
    double t = ((x1 - x3)*(y3 - y4) - (y1 - y3)*(x3 - x4)) / D;
    double u = ((x1 - x3)*(y1 - y2) - (y1 - y3)*(x1 - x2)) / D;
    // se t ∈ [0,1], o ponto pertence ao segmento 1; se u ∈ [0,1], o ponto pertence ao segmento 2 

    return (t >= 0 && t <= 1 && u >= 0 && u <= 1);
}

static bool sob_cc(FORMA a, FORMA b){
    assert(a && b);
    stForma *circuloA = (stForma*)a;
    stForma *circuloB = (stForma*)b;
    assert(circuloA->tipo == 'c' && circuloB->tipo == 'c');

    double xA = getX_circulo(circuloA->handle);
    double yA = getY_circulo(circuloA->handle);
    double rA = getR_circulo(circuloA->handle);
    double xB = getX_circulo(circuloB->handle);
    double yB = getY_circulo(circuloB->handle);
    double rB = getR_circulo(circuloB->handle);

    double dx = xB - xA;
    double dy = yB - yA;
    double distAB = hypot(dx, dy);
    double rs = rA + rB;

    return distAB <= rs;
}

static bool sob_cr(FORMA a, FORMA b){
    assert(a && b);
    stForma *circulo = (stForma*)a;
    stForma *retangulo = (stForma*)b;
    assert(circulo->tipo == 'c' && retangulo->tipo == 'r');

    double xC = getX_circulo(circulo->handle);
    double yC = getY_circulo(circulo->handle);
    double rC = getR_circulo(circulo->handle);

    double xR = getX_retangulo(retangulo->handle);
    double yR = getY_retangulo(retangulo->handle);
    double wR = getW_retangulo(retangulo->handle);
    double hR = getH_retangulo(retangulo->handle);

    double nx = fmax(xR, fmin(xC, xR + wR));
    double ny = fmax(yR, fmin(yC, yR + hR));

    double dx = nx - xC;
    double dy = ny - yC;

    return hypot(dx, dy) <= rC;
}

static bool sob_cl(FORMA a, FORMA b){
    assert(a && b);
    stForma *circulo = (stForma*)a;
    stForma *linha = (stForma*)b;
    assert(circulo->tipo == 'c' && linha->tipo == 'l');

    double xC = getX_circulo(circulo->handle);
    double yC = getY_circulo(circulo->handle);
    double rC = getR_circulo(circulo->handle);

    double xL1 = getX1_linha(linha->handle);
    double yL1 = getY1_linha(linha->handle);
    double xL2 = getX2_linha(linha->handle);
    double yL2 = getY2_linha(linha->handle);

    double distancia = dist_ponto_segmento(xC, yC, xL1, yL1, xL2, yL2);

    return distancia <= rC;
}

static bool sob_ct(FORMA a, FORMA b){
    assert(a && b);
    stForma *circulo = (stForma*)a;  
    stForma *texto = (stForma*)b;  
    assert(circulo->tipo == 'c' && texto->tipo == 't');

    double xC = getX_circulo(circulo->handle);
    double yC = getY_circulo(circulo->handle);
    double rC = getR_circulo(circulo->handle);

    double xT = getX_texto(texto->handle);
    double yT = getY_texto(texto->handle);
    const char *txt = getTXTO_texto(texto->handle);
    char ancora = getA_texto(texto->handle);

    double comprimento = 10.0 * (txt ? strlen(txt) : 0);

    double x1, y1, x2, y2;

    if (ancora == 'i'){         
        x1 = xT;        
        y1 = yT;
        x2 = xT + comprimento; 
        y2 = yT;

    } else if (ancora == 'f'){   
        x1 = xT - comprimento; 
        y1 = yT;
        x2 = xT;         
        y2 = yT;

    } else {                     
        x1 = xT - comprimento/2; 
        y1 = yT;
        x2 = xT + comprimento/2; 
        y2 = yT;
    }

    double distancia = dist_ponto_segmento(xC, yC, x1, y1, x2, y2);

    return distancia <= rC;
}

static bool sob_rr(FORMA a, FORMA b){
    assert(a && b);
    stForma *retanguloA = (stForma*)a, *retanguloB = (stForma*)b;
    assert(retanguloA->tipo == 'r' && retanguloB->tipo == 'r');

    double xA = getX_retangulo(retanguloA->handle);
    double yA = getY_retangulo(retanguloA->handle);
    double wA = getW_retangulo(retanguloA->handle);
    double hA = getH_retangulo(retanguloA->handle);

    double xB = getX_retangulo(retanguloB->handle);
    double yB = getY_retangulo(retanguloB->handle);
    double wB = getW_retangulo(retanguloB->handle);
    double hB = getH_retangulo(retanguloB->handle);

    const double eps = 1e-9;
    bool separado =
    (xA + wA < xB - eps) || (xB + wB < xA - eps) ||
    (yA + hA < yB - eps) || (yB + hB < yA - eps);

    return !separado; 
}

static bool sob_rl(FORMA a, FORMA b){
    assert(a && b);
    stForma *retangulo = (stForma*)a, *linha = (stForma*)b;
    assert(retangulo->tipo == 'r' && linha->tipo == 'l');

    double xR = getX_retangulo(retangulo->handle);
    double yR = getY_retangulo(retangulo->handle);
    double wR = getW_retangulo(retangulo->handle);
    double hR = getH_retangulo(retangulo->handle);

    double x1 = getX1_linha(linha->handle);
    double y1 = getY1_linha(linha->handle);
    double x2 = getX2_linha(linha->handle);
    double y2 = getY2_linha(linha->handle);

    if (ponto_no_retangulo(x1, y1, xR, yR, wR, hR) || ponto_no_retangulo(x2, y2, xR, yR, wR, hR)) return true;

    double ax = xR,      ay = yR;
    double bx = xR + wR, by = yR;
    double cx = xR + wR, cy = yR + hR;
    double dx = xR,      dy = yR + hR;

    if (seg_intersect(x1, y1, x2, y2, ax, ay, bx, by)) return true;
    if (seg_intersect(x1, y1, x2, y2, bx, by, cx, cy)) return true;
    if (seg_intersect(x1, y1, x2, y2, cx, cy, dx, dy)) return true; 
    if (seg_intersect(x1, y1, x2, y2, dx, dy, ax, ay)) return true; 

    return false;
}

static bool sob_rt(FORMA a, FORMA b){
    assert(a && b);
    stForma *retangulo = (stForma*)a;  
    stForma *texto = (stForma*)b;  
    assert(retangulo->tipo == 'r' && texto->tipo == 't');

    double xR = getX_retangulo(retangulo->handle);
    double yR = getY_retangulo(retangulo->handle);
    double wR = getW_retangulo(retangulo->handle);
    double hR = getH_retangulo(retangulo->handle);

    double xT = getX_texto(texto->handle);
    double yT = getY_texto(texto->handle);
    const char *txt = getTXTO_texto(texto->handle);
    char ancora = getA_texto(texto->handle);

    double comprimento = 10.0 * (txt ? strlen(txt) : 0);
    double x1, y1 = yT, x2, y2 = yT;

    if (ancora == 'i'){        
        x1 = xT;
        x2 = xT + comprimento;
    }
    else if (ancora == 'f'){     
        x1 = xT - comprimento;
        x2 = xT;
    }
    else {                      
        x1 = xT - comprimento / 2.0;
        x2 = xT + comprimento / 2.0;
    }

    if (ponto_no_retangulo(x1, y1, xR, yR, wR, hR) || ponto_no_retangulo(x2, y2, xR, yR, wR, hR)) return true;

    double ax = xR,      ay = yR;
    double bx = xR + wR, by = yR;
    double cx = xR + wR, cy = yR + hR;
    double dx = xR,      dy = yR + hR;

    if (seg_intersect(x1, y1, x2, y2, ax, ay, bx, by)) return true;
    if (seg_intersect(x1, y1, x2, y2, bx, by, cx, cy)) return true;
    if (seg_intersect(x1, y1, x2, y2, cx, cy, dx, dy)) return true; 
    if (seg_intersect(x1, y1, x2, y2, dx, dy, ax, ay)) return true; 

    return false;
}

static bool sob_ll(FORMA a, FORMA b){
    assert(a && b);
    stForma *linha1 = (stForma*)a;
    stForma *linha2 = (stForma*)b;
    assert(linha1->tipo == 'l' && linha2->tipo == 'l');

    double x1 = getX1_linha(linha1->handle);
    double y1 = getY1_linha(linha1->handle);
    double x2 = getX2_linha(linha1->handle);
    double y2 = getY2_linha(linha1->handle);

    double x3 = getX1_linha(linha2->handle);
    double y3 = getY1_linha(linha2->handle);
    double x4 = getX2_linha(linha2->handle);
    double y4 = getY2_linha(linha2->handle);

    return seg_intersect(x1, y1, x2, y2, x3, y3, x4, y4);
}


static bool sob_lt(FORMA a, FORMA b){
    assert(a && b);
    stForma *linha = (stForma*)a;  
    stForma *texto = (stForma*)b;  
    assert(linha->tipo == 'l' && texto->tipo == 't');

    double x1 = getX1_linha(linha->handle);
    double y1 = getY1_linha(linha->handle);
    double x2 = getX2_linha(linha->handle);
    double y2 = getY2_linha(linha->handle);

    double xT = getX_texto(texto->handle);
    double yT = getY_texto(texto->handle);
    const char *txt = getTXTO_texto(texto->handle);
    char ancora = getA_texto(texto->handle);

    double comprimento = 10.0 * (txt ? strlen(txt) : 0);
    double x3, y3 = yT, x4, y4 = yT;

    if (ancora == 'i'){          
        x3 = xT;
        x4 = xT + comprimento;
    }
    else if (ancora == 'f'){    
        x3 = xT - comprimento;
        x4 = xT;
    } else {                       
        x3 = xT - comprimento / 2.0;
        x4 = xT + comprimento / 2.0;
    }

    return seg_intersect(x1, y1, x2, y2, x3, y3, x4, y4);
}


static bool sob_tt(FORMA a, FORMA b){
    assert(a && b);
    stForma *texto1 = (stForma*)a, *texto2 = (stForma*)b;
    assert(texto1->tipo == 't' && texto2->tipo == 't');

    double xT1 = getX_texto(texto1->handle);
    double yT1 = getY_texto(texto1->handle);
    const char *s1 = getTXTO_texto(texto1->handle);
    char a1 = getA_texto(texto1->handle);
    double comprimento1 = 10.0 * (s1 ? strlen(s1) : 0);

    double x1, y1 = yT1, x2, y2 = yT1;
    if (a1 == 'i'){
        x1 = xT1;
        x2 = xT1 + comprimento1;
    } else if (a1 == 'f'){
        x1 = xT1 - comprimento1;
        x2 = xT1;
    } else {
        x1 = xT1 - comprimento1/2;
        x2 = xT1 + comprimento1/2;
    }

    double xT2 = getX_texto(texto2->handle);
    double yT2 = getY_texto(texto2->handle);
    const char *s2 = getTXTO_texto(texto2->handle);
    char a2 = getA_texto(texto2->handle);
    double comprimento2 = 10.0 * (s2 ? strlen(s2) : 0);

    double x3, y3 = yT2, x4, y4 = yT2;
    if (a2 == 'i'){ 
        x3 = xT2;       
        x4 = xT2 + comprimento2;
    } else if (a2 == 'f'){
        x3 = xT2 - comprimento2;
        x4 = xT2;
    } else {
        x3 = xT2 - comprimento2/2; x4 = xT2 + comprimento2/2;
    }

    return seg_intersect(x1, y1, x2, y2, x3, y3, x4, y4);
}


bool sobrepoe_formas(FORMA a, FORMA b){
    assert(a && b);
    stForma *A = (stForma*)a;
    stForma *B = (stForma*)b;

    switch (A->tipo){
    case 'c':
        switch (B->tipo){
        case 'c': return sob_cc(a, b);
        case 'r': return sob_cr(a, b);
        case 'l': return sob_cl(a, b);
        case 't': return sob_ct(a, b);
        default: break;
        }
        break;

    case 'r': 
        switch (B->tipo){
        case 'c': return sob_cr(b, a);
        case 'r': return sob_rr(a, b);
        case 'l': return sob_rl(a, b);
        case 't': return sob_rt(a, b);
        default: break;
        }
        break;

    case 'l': 
        switch (B->tipo){
        case 'c': return sob_cl(b, a); 
        case 'r': return sob_rl(b, a); 
        case 'l': return sob_ll(a, b);
        case 't': return sob_lt(a, b);
        default: break;
        }
        break;

    case 't': 
        switch (B->tipo){
        case 'c': return sob_ct(b, a); 
        case 'r': return sob_rt(b, a); 
        case 'l': return sob_lt(b, a); 
        case 't': return sob_tt(a, b);
        default: break;
        }
        break;
    }

    return false;
}

FORMA clonar_forma(FORMA f){
    if (!f) return NULL;

    stForma *forma = (stForma*)f;
    char tipo = forma->tipo;
    void *hand = forma->handle;

    int novo_id = gerar_id_clone();

    switch (tipo){
        case 'c': {
            double x = getX_circulo(hand);
            double y = getY_circulo(hand);
            double r = getR_circulo(hand);
            const char *corb = getCORB_circulo(hand);
            const char *corp = getCORP_circulo(hand);
            CIRCULO c = criar_circulo(novo_id, x, y, r, corb, corp);
            if (!c) return NULL;
            return criar_forma('c', c);
        }

        case 'r': {
            double x = getX_retangulo(hand);
            double y = getY_retangulo(hand);
            double w = getW_retangulo(hand);
            double h = getH_retangulo(hand);
            const char *corb = getCORB_retangulo(hand);
            const char *corp = getCORP_retangulo(hand);
            RETANGULO r = criar_retangulo(novo_id, x, y, w, h, corb, corp);
            if (!r) return NULL;
            return criar_forma('r', r);
        }

        case 'l': {
            double x1 = getX1_linha(hand);
            double y1 = getY1_linha(hand);
            double x2 = getX2_linha(hand);
            double y2 = getY2_linha(hand);
            const char *cor = getCOR_linha(hand);
            LINHA l = criar_linha(novo_id, x1, y1, x2, y2, cor);
            if (!l) return NULL;
            return criar_forma('l', l);
        }

        case 't': {
            double x = getX_texto(hand);
            double y = getY_texto(hand);
            const char *corb = getCORB_texto(hand);
            const char *corp = getCORP_texto(hand);
            char a = getA_texto(hand);
            const char *txt = getTXTO_texto(hand);
            TEXTO t = criar_texto(novo_id, x, y, corb, corp, a, txt);
            if (!t) return NULL;
            return criar_forma('t', t);
        }

        default:
            return NULL;
    }
}

static const struct { const char *nome; const char *hex; } kCoresSVG[] = {
    {"aliceblue",         "#F0F8FF"}, {"antiquewhite",      "#FAEBD7"}, {"aqua",              "#00FFFF"}, {"aquamarine",        "#7FFFD4"},
    {"azure",             "#F0FFFF"}, {"beige",             "#F5F5DC"}, {"bisque",            "#FFE4C4"}, {"black",             "#000000"},
    {"blanchedalmond",    "#FFEBCD"}, {"blue",              "#0000FF"}, {"blueviolet",        "#8A2BE2"}, {"brown",             "#A52A2A"},
    {"burlywood",         "#DEB887"}, {"cadetblue",         "#5F9EA0"}, {"chartreuse",        "#7FFF00"}, {"chocolate",         "#D2691E"},
    {"coral",             "#FF7F50"}, {"cornflowerblue",    "#6495ED"}, {"cornsilk",          "#FFF8DC"}, {"crimson",           "#DC143C"},
    {"cyan",              "#00FFFF"}, {"darkblue",          "#00008B"}, {"darkcyan",          "#008B8B"}, {"darkgoldenrod",     "#B8860B"},
    {"darkgray",          "#A9A9A9"}, {"darkgreen",         "#006400"}, {"darkgrey",          "#A9A9A9"}, {"darkkhaki",         "#BDB76B"},
    {"darkmagenta",       "#8B008B"}, {"darkolivegreen",    "#556B2F"}, {"darkorange",        "#FF8C00"}, {"darkorchid",        "#9932CC"},
    {"darkred",           "#8B0000"}, {"darksalmon",        "#E9967A"}, {"darkseagreen",      "#8FBC8F"}, {"darkslateblue",     "#483D8B"},
    {"darkslategray",     "#2F4F4F"}, {"darkslategrey",     "#2F4F4F"}, {"darkturquoise",     "#00CED1"}, {"darkviolet",        "#9400D3"},
    {"deeppink",          "#FF1493"}, {"deepskyblue",       "#00BFFF"}, {"dimgray",           "#696969"}, {"dimgrey",           "#696969"},
    {"dodgerblue",        "#1E90FF"}, {"firebrick",         "#B22222"}, {"floralwhite",       "#FFFAF0"}, {"forestgreen",       "#228B22"},
    {"fuchsia",           "#FF00FF"}, {"gainsboro",         "#DCDCDC"}, {"ghostwhite",        "#F8F8FF"}, {"gold",              "#FFD700"},
    {"goldenrod",         "#DAA520"}, {"gray",              "#808080"}, {"green",             "#008000"}, {"greenyellow",       "#ADFF2F"},
    {"grey",              "#808080"}, {"honeydew",          "#F0FFF0"}, {"hotpink",           "#FF69B4"}, {"indianred",         "#CD5C5C"},
    {"indigo",            "#4B0082"}, {"ivory",             "#FFFFF0"}, {"khaki",             "#F0E68C"}, {"lavender",          "#E6E6FA"},
    {"lavenderblush",     "#FFF0F5"}, {"lawngreen",         "#7CFC00"}, {"lemonchiffon",      "#FFFACD"}, {"lightblue",         "#ADD8E6"},
    {"lightcoral",        "#F08080"}, {"lightcyan",         "#E0FFFF"}, {"lightgoldenrodyellow","#FAFAD2"}, {"lightgray",         "#D3D3D3"},
    {"lightgreen",        "#90EE90"}, {"lightgrey",         "#D3D3D3"}, {"lightpink",         "#FFB6C1"}, {"lightsalmon",       "#FFA07A"},
    {"lightseagreen",     "#20B2AA"}, {"lightskyblue",      "#87CEFA"}, {"lightslategray",    "#778899"}, {"lightslategrey",    "#778899"},
    {"lightsteelblue",    "#B0C4DE"}, {"lightyellow",       "#FFFFE0"}, {"lime",              "#00FF00"}, {"limegreen",         "#32CD32"},
    {"linen",             "#FAF0E6"}, {"magenta",           "#FF00FF"}, {"maroon",            "#800000"}, {"mediumaquamarine",  "#66CDAA"},
    {"mediumblue",        "#0000CD"}, {"mediumorchid",      "#BA55D3"}, {"mediumpurple",      "#9370DB"}, {"mediumseagreen",    "#3CB371"},
    {"mediumslateblue",   "#7B68EE"}, {"mediumspringgreen", "#00FA9A"}, {"mediumturquoise",   "#48D1CC"}, {"mediumvioletred",   "#C71585"},
    {"midnightblue",      "#191970"}, {"mintcream",         "#F5FFFA"}, {"mistyrose",         "#FFE4E1"}, {"moccasin",          "#FFE4B5"},
    {"navajowhite",       "#FFDEAD"}, {"navy",              "#000080"}, {"oldlace",           "#FDF5E6"}, {"olive",             "#808000"},
    {"olivedrab",         "#6B8E23"}, {"orange",            "#FFA500"}, {"orangered",         "#FF4500"}, {"orchid",            "#DA70D6"},
    {"palegoldenrod",     "#EEE8AA"}, {"palegreen",         "#98FB98"}, {"paleturquoise",     "#AFEEEE"}, {"palevioletred",     "#DB7093"},
    {"papayawhip",        "#FFEFD5"}, {"peachpuff",         "#FFDAB9"}, {"peru",              "#CD853F"}, {"pink",              "#FFC0CB"},
    {"plum",              "#DDA0DD"}, {"powderblue",        "#B0E0E6"}, {"purple",            "#800080"}, {"rebeccapurple",     "#663399"},
    {"red",               "#FF0000"}, {"rosybrown",         "#BC8F8F"}, {"royalblue",         "#4169E1"}, {"saddlebrown",       "#8B4513"},
    {"salmon",            "#FA8072"}, {"sandybrown",        "#F4A460"}, {"seagreen",          "#2E8B57"}, {"seashell",          "#FFF5EE"},
    {"sienna",            "#A0522D"}, {"silver",            "#C0C0C0"}, {"skyblue",           "#87CEEB"}, {"slateblue",         "#6A5ACD"},
    {"slategray",         "#708090"}, {"slategrey",         "#708090"}, {"snow",              "#FFFAFA"}, {"springgreen",       "#00FF7F"},
    {"steelblue",         "#4682B4"}, {"tan",               "#D2B48C"}, {"teal",              "#008080"}, {"thistle",           "#D8BFD8"},
    {"tomato",            "#FF6347"}, {"turquoise",         "#40E0D0"}, {"violet",            "#EE82EE"}, {"wheat",             "#F5DEB3"},
    {"white",             "#FFFFFF"}, {"whitesmoke",        "#F5F5F5"}, {"yellow",            "#FFFF00"}, {"yellowgreen",       "#9ACD32"}
};

const char* nome_para_hex(const char* cor) {
    if (cor && cor[0] == '#' && strlen(cor) == 7) return cor; 

    size_t n = sizeof kCoresSVG / sizeof kCoresSVG[0];
    for (size_t i = 0; i < n; ++i) {             
        if (strcmp(cor, kCoresSVG[i].nome) == 0) 
            return kCoresSVG[i].hex;             
    }
    return cor; 
}

const char* hex_para_nome(const char *hex){
    if (!hex) return "(null)";
    if (hex[0] != '#') return hex;

    for (size_t i = 0; i < sizeof kCoresSVG / sizeof kCoresSVG[0]; ++i){
        if (strcmp(hex, kCoresSVG[i].hex) == 0)
            return kCoresSVG[i].nome;
    }

    return hex;
}

static bool comp_hex(const char *src, char out[8]){
    if (!src) return false;

    const char *hex = nome_para_hex(src);
    if (!hex || hex[0] != '#' || strlen(hex) != 7) return false;

    int r, g, b;
    if (sscanf(hex, "#%02x%02x%02x", &r, &g, &b) != 3) return false;

    r = 255 - r;
    g = 255 - g;
    b = 255 - b;

    snprintf(out, 8, "#%02X%02X%02X", r, g, b);
    return true;
}

void trocar_cores(FORMA i, FORMA j){
    if (!i || !j) return;

    char ti = getTipo_forma(i);
    char tj = getTipo_forma(j);

    if (ti == 'l' && tj == 'l'){
        const char *ci = getCOR_linha(getHandle_forma(i));
        const char *cj = getCOR_linha(getHandle_forma(j));

        char ci_comp[8], cj_comp[8];
        const char *nova_i = cj;
        const char *nova_j = ci;

        if (comp_hex(cj, ci_comp)) nova_i = ci_comp;
        if (comp_hex(ci, cj_comp)) nova_j = cj_comp;

        setCOR_linha(getHandle_forma(i), nova_i);
        setCOR_linha(getHandle_forma(j), nova_j);
        return;
    }

    const char *src = NULL;
    const char *orig = NULL;
    switch (ti){
        case 'c': src = getCORP_circulo(getHandle_forma(i)); break;
        case 'r': src = getCORP_retangulo(getHandle_forma(i)); break;
        case 't': src = getCORP_texto(getHandle_forma(i));    break;
        case 'l': {
            orig = getCOR_linha(getHandle_forma(i));      
            char comp [8];
            src = comp_hex(orig, comp) ? comp : orig;        
        } break; 
        default:  return;
    }

    switch (tj){
        case 'c': setCORB_circulo(getHandle_forma(j), src); break;
        case 'r': setCORB_retangulo(getHandle_forma(j), src); break;
        case 't': setCORB_texto(getHandle_forma(j), src); break;
        case 'l': setCOR_linha(getHandle_forma(j), src); break; 
    }
}

void inverter_cores(FORMA f){
    if (!f) return;
    char t = getTipo_forma(f);
    void *h = getHandle_forma(f);
    if (!h) return;

    switch (t){
        case 'c': {
            char *b = str_dup_c99(getCORB_circulo(h));
            char *p = str_dup_c99(getCORP_circulo(h));
            if (!b || !p){ free(b); free(p); return; }
            setCORB_circulo(h, p);  
            setCORP_circulo(h, b);
            free(b); free(p);
        } break;

        case 'r': {
            char *b = str_dup_c99(getCORB_retangulo(h));
            char *p = str_dup_c99(getCORP_retangulo(h));
            if (!b || !p){ free(b); free(p); return; }
            setCORB_retangulo(h, p);
            setCORP_retangulo(h, b);
            free(b); free(p);
        } break;

        case 't': {
            char *b = str_dup_c99(getCORB_texto(h));
            char *p = str_dup_c99(getCORP_texto(h));
            if (!b || !p){ free(b); free(p); return; }
            setCORB_texto(h, p);
            setCORP_texto(h, b);
            free(b); free(p);
        } break;

        case 'l': default:
            
        break;
    }
}