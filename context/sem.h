#ifndef SEM_H
#define SEM_H
/**
 * \file context.h
 * \brief Gestionnaire de semaphore
 * \author Elliot Vanegue et Gaëtan Deflandre
 * \version 1
 */
#include "context.h"

/** \struct
 * structure représentant un sémaphore
 */
struct sem_s{
  int sem_cpt;
  struct ctx_s * sem_ctx;
};

/**
 * Initialise un semaphore.
 *
 * \param[in] sem semaphore à initialiser.
 * \param[in] val valeur à placer dans sem_cpt.
 */
void sem_init(struct sem_s * sem, unsigned val);

/**
 * Décrémente le compteur d'un semaphore.
 *
 * \param[in] sem semaphore à décrémenter.
 */
void sem_down(struct sem_s * sem);

/**
 * Incrémente le compteur d'un semaphore.
 *
 * \param[in] sem semaphore à incrémenter.
 */
void sem_up(struct sem_s * sem);


#endif
