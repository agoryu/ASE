#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "hw/hardware.h"
#include "context/context.h"

#define STACK_SIZE 16384

#define CORE_IRQMAPPER 0x82
#define CORE_STATUS 0x80
#define CORE_ID 0x126
#define TIMER_ALARM 0xF8
#define TIMER_CLOCK 0xF0
#define TIMER_PARAM 0xF4
#define CORE_LOCK 0x98
#define CORE_UNLOCK 0x99

static void empty_it(){
    return;
}

static void again(){
    _mask(1);
    while(1);
    return;
}

static void print_number(){
    int num = _in(CORE_ID);
    int cpt = 0;
    int status;

    while(status != 1) status = _in(CORE_LOCK);

    printf("coeur %d debut\n", num);
    while(cpt++ != 10000000) {
        /*printf("%d", cpt);*/
    }
    printf("coeur %d fin\n", num);
    _out(CORE_UNLOCK, 0);
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

    IRQVECTOR[0] = again;
    IRQVECTOR[TIMER_IRQ] = print_number;

    for(i=1; i<4; i++) {
        _out(CORE_IRQMAPPER + i, 0xffffffff);
    }
    _out(CORE_IRQMAPPER, 0);

    _out(TIMER_ALARM, 0xffffffff - 20);
    _out(TIMER_PARAM, 1 << 6);

    _mask(1);
        
    _out(CORE_STATUS, 0xF);

    /*print_number();*/

    /*while(i != 10000000) i++;*/
    while(1);

    return 0;

}
