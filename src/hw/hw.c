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
    return 0;
  }

  /* Interreupt handlers */
  for(i=0; i<16; i++){
    IRQVECTOR[i] = empty_it;
  }

  /*IRQVECTOR[TIMER_IRQ] = yield;*/

  /* Allows all IT */
  _mask(1);

  return 1;
}
