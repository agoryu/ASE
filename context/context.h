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
#include "hw.h"

/**
 * \brief Define permettant de reconnaitre le contexte en cours 
 */
#define CTX_MAGIC 0xCAFEBABE

extern struct ctx_s *current_ctx;

/**
 * \enum ctx_state_e
 * Enumération des états que peut prendre une fonction 
 */
enum ctx_state_e {CTX_INIT, CTX_EXQ, CTX_END, CTX_STP};

/**
 * \typedef 
 * Représente une fonction qui retourne un void 
 */
typedef void (func_t) (void*);

/**
 * \struct 
 * Représente un contexte 
 */
struct ctx_s {
  void * ctx_esp;
  void * ctx_ebp;
  unsigned int ctx_state;
  func_t * ctx_f;
  void * ctx_arg;
  void * ctx_stack;
  unsigned ctx_magic;
  struct ctx_s * next;
  struct ctx_s * ctx_sem_next;
};


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
void start_sched(irq_handler_func_t f);


#endif
