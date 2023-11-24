//Bernardo Vieira Santos - 2220502 - 3WA
//Matheus Fonseca Vilella - 2210498 - 3WA
#include <stdio.h>
#include "cria_func.h"

void cria_func(void* f, DescParam params[], int n, unsigned char codigo[]) {
    unsigned char espacoPilha;
    if (n <= 2)
        espacoPilha = 0x10;
    else
        espacoPilha = 0x20;
    unsigned char prologo[] = {0x55, 0x48, 0x89, 0xe5, 0x48, 0x83, 0xec, espacoPilha, 0x89, 0xf8};
    unsigned char epilogo[] = {0xc9, 0xc3};
    
    int paramOcupado = 0;
    for (int i = 0; i < n; i++){
        switch (params[i].orig_val)
        {
        case PARAM:
            //codigo = {};
        break;

        case FIX:
            return;
        break;

        case IND:
            return;
        
        }
    }
}
