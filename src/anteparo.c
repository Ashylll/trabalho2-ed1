#include "anteparo.h"
#include "forma.h"
#include "lista.h"
#include "linha.h"
#include "circulo.h"
#include "retangulo.h"
#include "texto.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>



static int proximo_id_anteparo = -1;

int gerar_id_anteparo(void) {
    return proximo_id_anteparo--;
}

void circulo_para_anteparo(LISTA l_anteparos, CIRCULO c, const char v_h) {
    assert(l_anteparos && c && (v_h == 'v' || v_h == 'h'));

    const double r = getR_circulo(c);
    const double x = getX_circulo(c);
    const double y = getY_circulo(c);
    const char *cor = getCORB_circulo(c);
    ANTEPARO anteparo_circulo = NULL;

    if (v_h == 'v') {
        anteparo_circulo = criar_linha(gerar_id_anteparo(), x, y - r, x, y + r, cor);
        FORMA anteparo = criar_forma('l', anteparo_circulo);
        adicionar_lista(l_anteparos, anteparo);
    } else {
        anteparo_circulo = criar_linha(gerar_id_anteparo(), x - r, y, x + r, y, cor);
        FORMA anteparo = criar_forma('l', anteparo_circulo);
        adicionar_lista(l_anteparos, anteparo);
    }
}

void retangulo_para_anteparo(LISTA l_anteparos, RETANGULO r) {
    assert(l_anteparos && r);

    const double x = getX_retangulo(r);
    const double y = getY_retangulo(r);
    const double h = getH_retangulo(r);
    const double w = getW_retangulo(r);
    const char *cor = getCORB_retangulo(r);

    ANTEPARO segmento_superior = criar_linha(gerar_id_anteparo(), x, y, x + w, y, cor);
    ANTEPARO segmento_inferior = criar_linha(gerar_id_anteparo(), x, y + h, x + w, y + h, cor);
    ANTEPARO segmento_esquerdo = criar_linha(gerar_id_anteparo(), x, y, x, y + h, cor);
    ANTEPARO segmento_direito = criar_linha(gerar_id_anteparo(), x + w, y, x + w, y + h,cor);
    FORMA seg1 = criar_forma('l', segmento_superior);
    FORMA seg2 = criar_forma('l', segmento_inferior);
    FORMA seg3 = criar_forma('l', segmento_esquerdo);
    FORMA seg4 = criar_forma('l', segmento_direito);
    adicionar_lista(l_anteparos, seg1);
    adicionar_lista(l_anteparos, seg2);
    adicionar_lista(l_anteparos, seg3);
    adicionar_lista(l_anteparos, seg4);
}


void texto_para_anteparo(LISTA l_anteparos, TEXTO t) {
    assert (l_anteparos && t);
    const char ancora = getA_texto(t);
    const double xt = getX_texto(t);
    const double yt  = getY_texto(t);
    const double comprimento = tamanho_texto(t) * 10;
    const char *cor = getCORB_texto(t);

    switch (ancora) {
        case 'i': {
            ANTEPARO anteparo_texto = criar_linha(gerar_id_anteparo(), xt, yt, xt + comprimento, yt, cor);
            FORMA anteparo = criar_forma('l', anteparo_texto);
            adicionar_lista(l_anteparos, anteparo);

            break;
        }

        case 'm': {
            ANTEPARO anteparo_texto = criar_linha(gerar_id_anteparo(), xt - (comprimento/2), yt, xt + (comprimento/2), yt, cor);
            FORMA anteparo = criar_forma('l', anteparo_texto);
            adicionar_lista(l_anteparos, anteparo);

            break;
        }

        case 'f': {
            ANTEPARO anteparo_texto = criar_linha(gerar_id_anteparo(), xt - comprimento, yt, xt, yt, cor);
            FORMA anteparo = criar_forma('l', anteparo_texto);
            adicionar_lista(l_anteparos, anteparo);

            break;
        }

        default: {
            fprintf(stderr, "Erro: âncora '%c' inválida em texto_para_anteparo()\n", ancora);
            break;
        }
    }
}



void comando_anteparo(LISTA l_formas, LISTA l_anteparos, int i, const int j, const char v_h) {
    assert (v_h == 'v' || v_h == 'h');
    FORMA forma = NULL;

    for (; i <= j; i++) {
        forma = buscar_lista(l_formas, i);
        if (!forma) {
            fprintf(stderr, "Erro: forma de id %d nao encontrada em comando_anteparo()", i);
            continue;
        }
        const char tipo = getTipo_forma(forma);

        void *handle = getHandle_forma(forma);
        switch (tipo) {
            case 'c': {
                circulo_para_anteparo(l_anteparos, handle, v_h);
                remover_lista(l_formas, forma);
                destruir_forma(forma);

                break;
            }
            case 'r': {
                retangulo_para_anteparo(l_anteparos, handle);
                remover_lista(l_formas, forma);
                destruir_forma(forma);

                break;
            }

            case 'l': {
                ANTEPARO anteparo_linha = remover_lista(l_formas, forma);
                adicionar_lista(l_anteparos, anteparo_linha);

                break;
            }

            case 't': {
                texto_para_anteparo(l_anteparos, handle);
                remover_lista(l_formas, forma);
                destruir_forma(forma);

                break;
            }

            default: {
                fprintf(stderr, "Erro: tipo '%c' inválido em comando_anteparo()", tipo);
                break;
            }
        }
    }
}
