#ifndef _MI_SYSCALL_H_
#define _MI_SYSCALL_H_

#include <stdlib.h>
#include <stdio.h>
#include "hw/hardware.h"
#include <string.h>

#define MMU_ENABLE 	 1
#define MMU_IRQ		 13
#define MMU_CMD		 0x66
#define MMU_FAULT_ADDR	 0xCD
#define TLB_ADD_ENTRY	 0xCE
#define TLB_DEL_ENTRY	 0xDE
#define TLB_SIZE	 32
#define TLB_ENTRIES      0x800


/**
 * Taille des pages
 */
#define PAGE_SIZE        4096


/**
 * Nombre de pages pour la mémoire physique.
 */
#define PM_PAGES         256
/**
 * Taille de la mémoire physique.
 */
#define PM_SIZE          (PM_PAGES*PAGE_SIZE)
/**
 * Début de la zone de mémoire physique. 
 */
#define PM_BEGIN         *(unsigned*)&physical_memory
/**
 * Fin de la zone de mémoire physique.
 */
#define PM_END           (PM_BEGIN + PM_SIZE - 1)


/**
 * Nombre de pages pour la mémoire virtuelle.
 */
#define VM_PAGES         4096
/**
 * Taille de la mémoire virtuelle.
 */
#define VM_SIZE          (VM_PAGES*PAGE_SIZE)
/**
 * Début de la zone de mémoire virtuelle. 
 */
#define VM_BEGIN         *(unsigned*)&virtual_memory
/**
 * Fin de la zone de mémoire virtuelle. 
 */
#define VM_END           (VM_BEGIN + VM_SIZE - 1)


/**
 * Indice du handler permettant de basculer sur le contexte 0.
 */
#define SYSCALL_SWITCH_0 16
/**
 * Indice du handler permettant de basculer sur le contexte 1.
 */
#define SYSCALL_SWITCH_1 17
#define N 42

/**
 * \brief Structure des entrées de la TLB (Tranlation 
 * Lookaside Buffer).
 * \struct tlb_entity_s "mi_syscall.h"
 */
struct tlb_entity_s {
  /** R.F.U. */
  unsigned tlb_rfu :8;
  /** page virtuelle */
  unsigned tlb_vpage :12;
  /** page physique */
  unsigned tlb_ppage :8;
  /** accès exécution, écriture, lecture */
  unsigned tlb_acces :3;
  /** entrée utilisée ou non */
  unsigned tlb_active :1;
};


/**
 * Est Appelée lors d'une interruption matériel.
 */
void mmuhandler();

/**
 * Vérification de la validité de l'adresse virtuelle.
 * \param[in] vaddr L'adresse à tester.
 * \return 1 si l'adresse est dans l'espace des adresses virtuelles,
 * sinon 0.
 */
unsigned is_vaddr(const unsigned vaddr);

/**
 *
 */
int ppage_of_vaddr(const int process, const unsigned vaddr);

/**
 * Initialise les interruptions et met le système en mode 
 * utilisateur.
 * \return 1 si correctement initialiser sinon 0.
 */
unsigned init();

/**
 * Fonction de test, qui crée des interruptions logiciels.
 */
void user();

#endif /* _MI_SYSCALL_H_ */
