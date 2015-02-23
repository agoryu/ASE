#ifndef CONTEXT_H
#define CONTEXT_H

/**
 * \file context.h
 * \brief Gestionnaire de contexte
 * \author Elliot Vanegue et Gaëtan Deflandre
 * \version 1
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "hw/hw.h"

/**
 * \brief Define permettant de reconnaitre le contexte en cours 
 */
#define CTX_MAGIC 0xCAFEBABE

/**
 * Tableau des contextes courants pour chacun des coeurs.
 */
extern struct ctx_s * ctx_current[CORE_NCORE];

/**
 * \enum ctx_state_e
 * Enumération des états que peut prendre un contexte. 
 */
enum ctx_state_e {CTX_INIT, CTX_EXQ, CTX_END, CTX_STP};

/**
 * \typedef 
 * Représente une fonction qui retourne un void 
 */
typedef void (func_t) (void*);

/**
 * \struct ctx_s
 * Représente un contexte 
 */
struct ctx_s {
    /** TODO verif -> Pointeur du haut de pile. */
    void * ctx_esp;
    /** TODO verif -> Pointeur du bas de pile. */
    void * ctx_ebp;
    /** Etat du contexte. */
    unsigned ctx_state;
    /** Fonction du contexte. */
    func_t * ctx_f;
    /** Arguments de la fonction du contexte. */
    void * ctx_arg;
    /** Pile du contexte. */
    void * ctx_stack;
    /** Nombre magique. */
    unsigned ctx_magic;
    /** Pointeur sur le contexte suivant. */
    struct ctx_s * next;
    /** TODO savoir ce champ. */
    struct ctx_s * ctx_sem_next;
};

/**
 * \struct struct ctxs_by_core_s
 * Liste des contextes appartenant à un coeur.
 */
struct ctxs_by_core_s {
    /** Nombre de contextes pour ce coeur. */
    unsigned ctxs_lenght;
    /** L'anneau de contextes pour ce coeur. */
    struct ctx_s * ctxs_ring;
};


/**
 * Initialise le système de contexte.
 *
 * \return 0 en cas d'erreur, sinon 1.
 */
unsigned init_ctxsys();

/**
 * Crée un nouveau contexte et le mémorise dans une structure chaînée 
 * circulaire.
 *
 * \param[in] stack_size La taille de la pile pour le contexte créé.
 * \param[in] f La fonction du contexte.
 * \param[in] args L'argument de la fonction du contexte.
 * \return 0 en cas d'erreur, sinon 1.
 */
int create_ctx(int stack_size, func_t f, void * args);

/**
 * Permet au contexte courant de passer la main à un autre contexte. Ce 
 * dernier étant déterminé par l'ordonnancement.
 */
void yield();

/**
 * Installe les gestionnaires d’interruptions et initialise le matériel.
 * \param[in] f Fonction associé à l'interruption.
 */
/*void start_sched(irq_handler_func_t f);*/


#endif
