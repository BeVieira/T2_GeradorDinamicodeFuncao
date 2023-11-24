#include <stdio.h>
#include <stdlib.h>
#include "cria_func.h"

typedef int (*func_ptr) (int x);

int mult(int x, int y) {
	return x * y;
}

int main(void) {
	DescParam params[2];
	func_ptr f_mult;
	int i;
	unsigned char codigo[500];

	params[0].tipo_val = INT_PAR; /* o primeiro parametro de mult e int */
	params[0].orig_val = PARAM;   /* a nova funcao repassa seu parametro */

	params[1].tipo_val = INT_PAR; /* o segundo parametro de mult e int */
	params[1].orig_val = FIX;     /* a nova funcao passa para mult a constante 10 */
	params[1].valor.v_int = 10;

	cria_func(mult, params, 2, codigo);
	f_mult = (func_ptr)codigo;

	for (i = 1; i <= 10; i++) {
		printf("%d\n", f_mult(i)); /* a nova funcao so recebe um argumento */
	}

	return 0;
}