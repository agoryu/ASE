#ifndef _VOL_H
#define _VOL_H

/**
 * \file vol.h
 * \brief Gestionnaire de bloc
 * \author Elliot Vanegue et Gaëtan Deflandre
 * \version 1
 */

#include "filesys/mbr.h"


/** Position du super dans la liste de bloc */
#define SUPER_BLOC 0
/** Magique utilisé pour Le bloc super */
#define SUPER_MAGIC 0xDEADC0DE

/** Numero du volume courant */
#define CURRENT_VOLUME 0

/** Nombre de fonction vide a créé */
#define NB_EMPTY_FUNCTION 16

/** Le volume courrant utile pour les couches supérieurs */
extern int current_vol;


/**
 * \brief Représente le premier bloc qui indique 
 *	où se trouve le premier bloc libre
 * \struct super_s "vol.h"  
 */
struct super_s {
  /** numéro magique */
  unsigned super_magic;
  /** premier bloc libre dans la liste de bloc */
  unsigned super_first_free;
  /** nombre de bloc libre dans la liste de bloc */
  unsigned super_n_free;
  /** inumber du root*/
  unsigned super_iroot;
};


/**
 * \brief Représente un ensemble contigue de bloc libre
 * \struct free_bloc_s "vol.h" 
 */
struct free_bloc_s {
  /** nombre de bloc libre à la suite */
  unsigned fb_n_free;
  /** prochain ensemble de bloc libre */
  unsigned fb_next;
};

/**
 * Initialise le super bloc d'un volume
 * \param[in] vol numéro du volume à initialiser
 */
void init_super(const unsigned vol);

/**
 * Initialise le répertoire root pour le volume monté.
 */
unsigned init_root(const unsigned int vol);

/* TODO
 * Monte le volume \a vol comme volume courrant.
 * \param[in] vol Le volume a monté.
 * \return 1 si la fonction c'est bien passé, 0 sinon.
 */
/*unsigned mount(unsigned int vol);*/

/* TODO
 * Démonte le volume courrant et monte le volume système.
 * \return 1 si la fonction c'est bien passé, 0 sinon.
 */
/*unsigned umount();*/

/**
 * Charge le super bloc dans une variable globale
 * \param[in] vol numéro du volume à charger
 * \return 1 si le chargement a fonctionné, 0 sinon
 */
unsigned load_super(const unsigned vol);

/**
 * Initialise le répertoire root pour le volume monté avec le
 * inumber \a root_inumber.
 * \param[in] root_inumber inumber de la racine.
 */
unsigned init_root(const unsigned root_inumber);

/**
 * TODO doc
 */
unsigned save_current_super();

/**
 * Retourne le numéro d'un bloc libre sur le volume 
 * courrant si disponible, sinon retourne 0 lorsqu'il n'y a 
 * plus de blocs libres.
 * \return le numéro d'un bloc libre, <code>0</code> sinon.
 */
unsigned int new_bloc();


/**
 * \brief Libère un bloc dans la liste de bloc
 * \param[in] bloc numéro du bloc à libérer
 */
void free_bloc(const unsigned int bloc);

/**
 * \brief Libère plusieurs blocs dans la liste de bloc
 * \param[in] bloc numéro du bloc à libérer
 */
void free_blocs(const unsigned * blocs, const size_t size);

/**
 * Vérifie si le disque courant est rempli
 * \return 1 si le disque est rempli, 0 sinon
 */
unsigned int is_full();


/**
 * Retourne le nombre de bloc libre dans le volume courant 
 * \return le nombre de bloc libre
 */
unsigned int get_nb_free_bloc();


#endif
