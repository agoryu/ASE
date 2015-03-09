/* ------------------------------
   $Id: hw.c,v 1.5 2006/10/24 07:21:59 marquet Exp $
   ------------------------------------------------------------

   Basic hardware emulation.
   Philippe Marquet, Dec 2003
   
*/

#include <stdio.h>

#include "context/context.h"
#include "hw/hw.h"
#include "hw/hardware.h"

static void again(){
    _mask(1);
    while(1);
    return;
}

static void empty_it(){
    return;
}

void irq_disable() {
    _mask(15);
}

void irq_enable() {
    _mask(1);
}

unsigned boot() {

    unsigned i;

    /* init hardware */
    if(!init_hardware(HW_CONFIG)){
        fprintf(stderr, "FATAL: echec initialisation disque et coeurs.\n");
        return 0;
    }

    /* Interreupt handlers */
    for(i=0; i<16; i++){
        IRQVECTOR[i] = empty_it;
    }

    IRQVECTOR[0] = again;
    IRQVECTOR[TIMER_IRQ] = yield;

    for(i=1; i<CORE_NCORE; i++) {
        _out(CORE_IRQMAPPER + i, 0xffffffff);
    }
    _out(CORE_IRQMAPPER, 0);

    _out(TIMER_ALARM, 0xffffffff - 20);
    _out(TIMER_PARAM, 1 << 6);/*128 + 64 + 32 + 16 + 8);*/

    /* Allows all IT */
    _mask(1);

    _out(CORE_STATUS, 0xF);

    /* initialise le systeme de contexte multi coeur */
    if(!init_ctxsys()){
        fprintf(stderr, "FATAL: echec initialisation système de contextes.\n");
        return 0;
    }

    return 1;
}
