/* ------------------------------
   $Id: hw.h,v 1.1 2003/12/18 17:15:06 marquet Exp $
   ------------------------------------------------------------

   Basic hardware simulation
   Philippe Marquet, Dec 2003
   
*/

#ifndef _HW_H_
#define _HW_H_

/**
 * \file hw.h
 * \brief Créateur d'interuption
 * \author Philippe Marquet
 * \version 1
 */

typedef void (irq_handler_func_t)(void); 

#define TIMER_IRQ	2
/** Nom du fichier d'initialisation du materiel */
#define HW_CONFIG "etc/hardware.ini"

void setup_irq(unsigned int irq, irq_handler_func_t handler);
void start_hw();

void irq_disable();
void irq_enable(); 

/**
 * Initialise le matériel. Initialise le disque virtuel, le crée si 
 * nécessaire. Initialise les interruptions matériels. 
 */
unsigned boot();

#endif
