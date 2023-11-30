// Bernardo Vieira Santos - 2220502 - 3WA
// Matheus Fonseca Vilella - 2210498 - 3WA
#include "cria_func.h"
#include <stdio.h>
#include <string.h>

void cria_func(void* f, DescParam params[], int n, unsigned char codigo[]) {
    unsigned char prologo[] = { 0x55, 0x48, 0x89, 0xE5 };
    unsigned char registradores32x[] = { 0xbf, 0xbe, 0xba }; //Instruções movl $var, %reg-parametro
    unsigned char finalMem2Reg[] = { 0x39, 0x31, 0x11 }; //Instruções finais movl (%reg), %reg-parametro
    unsigned char epilogo[] = { 0xc9, 0xc3 };
    int amarrado[] = { 0, 0, 0 };
    int pos = 0;

    for (int i = 0; i < sizeof(prologo); i++)
        codigo[pos++] = prologo[i];

    // Manipulação dos registrados de parâmetros
    //  novaF(y) > f(FIX, y)
    if (n == 2) {
        if (params[0].orig_val == PARAM && params[1].orig_val != PARAM) {
            unsigned char comando[] = { 0x48, 0x89, 0xfe };
        }
    }

    if (n == 3) {
        for (int i = 0; i < n; i++)
            if (params[i].orig_val != PARAM)
                amarrado[i] = 1;

        // Casos que precisam ser mexidos
        //  novaF(x,z) > f(x,FIX,z)
        if (!amarrado[0] && amarrado[1] && !amarrado[2]) {
            if (params[2].tipo_val == PTR_PAR) {
                unsigned char comando[] = { 0x48, 0x89, 0xf2 };
            }
            else {
                unsigned char comando[] = { 0x89, 0xf2 };
            }
        }
        // novaF(y,z) > f(FIX,y,z)
        else if (amarrado[0] && !amarrado[1] && !amarrado[2]) {
            if (params[1].tipo_val == PTR_PAR && params[2].tipo_val == PTR_PAR) {
                unsigned char comando[] = { 0x48, 0x89, 0xf2, 0x48, 0x89, 0xfe };
            }
            else if (params[1].tipo_val == PTR_PAR && params[2].tipo_val == INT_PAR) {
                unsigned char comando[] = { 0x48, 0x89, 0xf2, 0x89, 0xfe };
            }
            else if (params[1].tipo_val == INT_PAR && params[2].tipo_val == PTR_PAR) {
                unsigned char comando[] = { 0x89, 0xf2, 0x48, 0x89, 0xfe };
            }
            else if (params[1].tipo_val == INT_PAR && params[2].tipo_val == INT_PAR) {
                unsigned char comando[] = { 0x89, 0xf2, 0x89, 0xfe };
            }

            // novaF(y) > f(FIX, y, FIX)
            else if (amarrado[0] && !amarrado[1] && amarrado[2]) {
                if (params[1].tipo_val == PTR_PAR) {
                    unsigned char comando[] = { 0x48, 0x89, 0xfe };
                }
                else if (params[1].tipo_val == INT_PAR) {
                    unsigned char comando[] = { 0x89, 0xfe };
                }

            }
            // novaF(z) > f(FIX, FIX, z)
            else if (amarrado[0] && amarrado[1] && !amarrado[2]) {
                if (params[2].tipo_val == PTR_PAR) {
                    unsigned char comando[] = { 0x48, 0x89, 0xfa };
                }
                else if (params[2].tipo_val == INT_PAR) {
                    unsigned char comando[] = { 0x89, 0xfa };
                }
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
                unsigned char comando2[] = { registradores32x[numParam], vetorInt[0], vetorInt[1], vetorInt[2], vetorInt[3] };
            }
            else if (params[numParam].orig_val == IND) {
                unsigned char vetorPtr[sizeof(double)];
                unsigned long ponteiro = (unsigned long)params[numParam].valor.v_ptr;
                for (int i = 0; i < sizeof(int); ++i)
                    //Passando o ponteiro para little-endian
                    vetorPtr[i] = (ponteiro >> (8 * i)) & 0xFF;
                // Usando  %rcx como registrador auxiliar para segurar o endereço do parametro indireto
                unsigned char comando2[] = { 0x48, 0xb9, vetorPtr[0], vetorPtr[1], vetorPtr[2], vetorPtr[3], vetorPtr[4], vetorPtr[5], vetorPtr[6], vetorPtr[7], 0x8b, finalMem2Reg[numParam] };
            }
        }
        else if (params[numParam].tipo_val == PTR_PAR) {
            //Mesma lógica só que para ponteiro
        }
    }
}