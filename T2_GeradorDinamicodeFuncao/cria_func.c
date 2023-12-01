// Bernardo Vieira Santos - 2220502 - 3WA
// Matheus Fonseca Vilella - 2210498 - 3WA
#include "cria_func.h"
#include <stdio.h>
#include <string.h>

int insereComando(unsigned char codigo[], unsigned char comando[], int pos, int tam) {
    for (int i = 0; i < tam; i++)
        codigo[pos++] = comando[i];
    return pos;
}


void cria_func(void* f, DescParam params[], int n, unsigned char codigo[]) {
    unsigned char prologo[] = { 0x55, 0x48, 0x89, 0xE5 }; // Push %rbp, movq %rsp, %rbp
    unsigned char registradoresParam[] = { 0xbf, 0xbe, 0xba }; //Instruções mov $const, %reg-parametro
    unsigned char finalMem2Reg[] = { 0x39, 0x31, 0x11 }; //Instruções finais mov (%reg), %reg-parametro
    unsigned char finalReg2Reg[] = { 0xdf, 0xde, 0xda }; //Instruções finais mov %reg, %reg-parametro
    unsigned char epilogo[] = { 0xff, 0xd0, 0xc9, 0xc3 }; //Chamar a função em %rax, leave e ret
    int amarrado[] = { 0, 0, 0 };
    int pos = 0;

    pos = insereComando(codigo, prologo, pos, sizeof(prologo));

    // Manipulação dos registrados de parâmetros
    //  novaF(y) > f(FIX, y)
    if (n == 2) {
        if (params[0].orig_val != PARAM && params[1].orig_val == PARAM) {
            if (params[0].tipo_val == PTR_PAR) {
                unsigned char comando0[] = { 0x48, 0x89, 0xfe };
                pos = insereComando(codigo, comando0, pos, sizeof(comando0));
            }
            else {
                unsigned char comando0[] = { 0x89, 0xfe };
                pos = insereComando(codigo, comando0, pos, sizeof(comando0));
            }
        }
    }
    else if (n == 3) {
        for (int i = 0; i < n; i++)
            if (params[i].orig_val != PARAM)
                amarrado[i] = 1;
        // Casos que precisam ser mexidos
        // novaF(x,z) > f(x,FIX,z)
        if (!amarrado[0] && amarrado[1] && !amarrado[2]) {
            if (params[2].tipo_val == PTR_PAR) {
                unsigned char comando0[] = { 0x48, 0x89, 0xf2 };
                pos = insereComando(codigo, comando0, pos, sizeof(comando0));
            }
            else {
                unsigned char comando0[] = { 0x89, 0xf2 };
                pos = insereComando(codigo, comando0, pos, sizeof(comando0));
            }
        }
        // novaF(y,z) > f(FIX,y,z)
        else if (amarrado[0] && !amarrado[1] && !amarrado[2]) {
            if (params[1].tipo_val == PTR_PAR && params[2].tipo_val == PTR_PAR) {
                unsigned char comando0[] = { 0x48, 0x89, 0xf2, 0x48, 0x89, 0xfe };
                pos = insereComando(codigo, comando0, pos, sizeof(comando0));
            }
            else if (params[1].tipo_val == PTR_PAR && params[2].tipo_val == INT_PAR) {
                unsigned char comando0[] = { 0x48, 0x89, 0xf2, 0x89, 0xfe };
                pos = insereComando(codigo, comando0, pos, sizeof(comando0));
            }
            else if (params[1].tipo_val == INT_PAR && params[2].tipo_val == PTR_PAR) {
                unsigned char comando0[] = { 0x89, 0xf2, 0x48, 0x89, 0xfe };
                pos = insereComando(codigo, comando0, pos, sizeof(comando0));
            }
            else if (params[1].tipo_val == INT_PAR && params[2].tipo_val == INT_PAR) {
                unsigned char comando0[] = { 0x89, 0xf2, 0x89, 0xfe };
                pos = insereComando(codigo, comando0, pos, sizeof(comando0));
            }
        }
        // novaF(y) > f(FIX, y, FIX)
        else if (amarrado[0] && !amarrado[1] && amarrado[2]) {
            if (params[1].tipo_val == PTR_PAR) {
                unsigned char comando0[] = { 0x48, 0x89, 0xfe };
                pos = insereComando(codigo, comando0, pos, sizeof(comando0));
            }
            else {
                unsigned char comando0[] = { 0x89, 0xfe };
                pos = insereComando(codigo, comando0, pos, sizeof(comando0));
            }
        }
        // novaF(z) > f(FIX, FIX, z)
        else if (amarrado[0] && amarrado[1] && !amarrado[2]) {
            if (params[2].tipo_val == PTR_PAR) {
                unsigned char comando0[] = { 0x48, 0x89, 0xfa };
                pos = insereComando(codigo, comando0, pos, sizeof(comando0));
            }
            else {
                unsigned char comando0[] = { 0x89, 0xfa };
                pos = insereComando(codigo, comando0, pos, sizeof(comando0));
            }
        }
    }
    //Atribuir aos parametros seus valores amarrados
    for (int numParam = 0; numParam < n; numParam++) {
        if (params[numParam].tipo_val == INT_PAR) {
            if (params[numParam].orig_val == FIX) {
                unsigned char vetorInt[sizeof(int)];
                for (int i = 0; i < sizeof(int); ++i)
                    //Passando o inteiro para little-endian
                    vetorInt[i] = (params[numParam].valor.v_int >> (8 * i)) & 0xFF;
                unsigned char comando1[] = { registradoresParam[numParam], vetorInt[0], vetorInt[1], vetorInt[2], vetorInt[3] };
                pos = insereComando(codigo, comando1, pos, sizeof(comando1));
            }
            else if (params[numParam].orig_val == IND) {
                unsigned char vetorPtr[sizeof(double)];
                unsigned long ponteiroAux = (unsigned long)params[numParam].valor.v_ptr;
                for (int i = 0; i < sizeof(double); ++i)
                    //Passando o ponteiro para little-endian
                    vetorPtr[i] = (ponteiroAux >> (8 * i)) & 0xFF;
                // Usando  %rcx (0xb9) como registrador auxiliar para segurar o endereço do parametro indireto
                unsigned char comando1[] = { 0x48, 0xb9, vetorPtr[0], vetorPtr[1], vetorPtr[2], vetorPtr[3], vetorPtr[4], vetorPtr[5], vetorPtr[6], vetorPtr[7], 0x8b, finalMem2Reg[numParam] };
                pos = insereComando(codigo, comando1, pos, sizeof(comando1));
            }
        }
        else if (params[numParam].tipo_val == PTR_PAR) {
            if (params[numParam].orig_val == FIX) {
                unsigned char vetorPtr[sizeof(double)];
                unsigned long ponteiroAux = (unsigned long)params[numParam].valor.v_ptr;
                for (int i = 0; i < sizeof(double); ++i)
                    //Passando o ponteiro para little-endian
                    vetorPtr[i] = (ponteiroAux >> (8 * i)) & 0xFF;
                unsigned char comando1[] = { 0x48, registradoresParam[numParam], vetorPtr[0], vetorPtr[1], vetorPtr[2], vetorPtr[3], vetorPtr[4], vetorPtr[5], vetorPtr[6], vetorPtr[7] };
                pos = insereComando(codigo, comando1, pos, sizeof(comando1));
            }
            else if (params[numParam].orig_val == IND) {
                unsigned char vetorPtr[sizeof(double)];
                unsigned long ponteiroAux = (unsigned long)params[numParam].valor.v_ptr;
                for (int i = 0; i < sizeof(double); ++i)
                    //Passando o ponteiro para little-endian
                    vetorPtr[i] = (ponteiroAux >> (8 * i)) & 0xFF;
                // Usando  %rbx (0xbb) como registrador auxiliar para segurar o endereço do parametro indireto
                unsigned char comando1[] = { 0x48, 0xbb, vetorPtr[0], vetorPtr[1], vetorPtr[2], vetorPtr[3], vetorPtr[4], vetorPtr[5], vetorPtr[6], vetorPtr[7], 0x48, 0x89, finalReg2Reg[numParam] };
                pos = insereComando(codigo, comando1, pos, sizeof(comando1));
            }
        }
    }

    unsigned char vetorPtr[sizeof(double)];
    unsigned long ponteiroFAux = (unsigned long)f;
    for (int i = 0; i < sizeof(double); ++i) {
        //Passando o ponteiro para little-endian
        vetorPtr[i] = (ponteiroFAux >> (8 * i)) & 0xFF;
    }
    //Mover o endereço da função para %rax
    unsigned char chamada[] = { 0x48, 0xb8, vetorPtr[0], vetorPtr[1], vetorPtr[2], vetorPtr[3], vetorPtr[4], vetorPtr[5], vetorPtr[6], vetorPtr[7] };
    pos = insereComando(codigo, chamada, pos, sizeof(chamada));
    //Chama a função e encerra o codigo
    pos = insereComando(codigo, epilogo, pos, sizeof(epilogo));
}
