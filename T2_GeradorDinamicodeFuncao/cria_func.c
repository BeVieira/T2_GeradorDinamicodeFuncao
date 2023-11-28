// Bernardo Vieira Santos - 2220502 - 3WA
// Matheus Fonseca Vilella - 2210498 - 3WA
#include "cria_func.h"
#include <stdio.h>
#include <string.h>

void cria_func(void* f, DescParam params[], int n, unsigned char codigo[]) {
    unsigned char prologo[] = { 0x55, 0x48, 0x89, 0xE5 };
    unsigned char epilogo[] = { 0xc9, 0xc3 };
    int amarrado[] = { 0, 0, 0 };
    int pos = 0;

    for (int i = 0; i < 4; i++, pos++)
        codigo[i] = prologo[i];

    // Manipulação dos registrados de parâmetros
    //  novaF(y) > f(FIX, y)
    if (n == 2)
        if (params[0].orig_val == PARAM && params[1].orig_val != PARAM) {
            unsigned char comando[] = { 0x48, 0x89, 0xfe };
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
}