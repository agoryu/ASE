#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hw/hardware.h"

#define CORE_STATUS 0x80
#define CORE_ID 0x126

static void empty_it(){
  return;
}

static void print_number(){
	int num = _in(CORE_ID);
	
	while(1) {
		int cpt = 0;
	printf("coeur n %d\n", num);

	while(cpt != 10000000) cpt++;
}
    return;
}

int main() {

	unsigned i;

	if(!init_hardware("etc/core.ini")){
		printf("pas retrouvé le .ini");
		return 0;
	}

	/* Interreupt handlers */
	for(i=0; i<16; i++){
		IRQVECTOR[i] = empty_it;
	}
	IRQVECTOR[0] = print_number;

	/*IRQVECTOR[TIMER_IRQ] = yield;*/

	/*_mask(1);*/
	
	_out(CORE_STATUS, 0xF);

	print_number();

	/*while(i != 10000000) i++;*/

	return 0;

}