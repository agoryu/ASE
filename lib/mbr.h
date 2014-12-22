#ifndef _MBR_H_
#define _MBR_H_

/**
 * \file mbr.h
 * \brief Gestionnaire de volumes
 * \author Elliot Vanegue et Gaëtan Deflandre
 * \version 1.2
 */

#include "drive.h"

/** Nombre maximum de volume dans le disque */
#define MAX_VOL 8

/** Nombre magique utilisé pour la structure mbr_s */
#define MBR_MAGIC 0xDEADC0DE

/** Taille en nombre d'octets d'un bloc */
#define BLOC_SIZE HDA_SECTORSIZE

/** Variable globale du Master Boot Record disponible pour les couches supérieurs */
extern struct mbr_s mbr;

/**
 * \Brief Type de volume
 * \enum vol_type_e mbr.h "mbr.h"
 */
enum vol_type_e {
  /** Volume de base pour le systeme de fichier */
  VOLT_PR, 
  /** Volume annexe du systeme de fichiers */
  VOLT_SND, 
  /** Autre type de volume qui ne peut etre associe au systeme de fichiers */
  VOLT_OTHER
};

/**
 * \brief Structure qui decrit un volume
 * \struct vol_s mbr.h "mbr.h"
 */
struct vol_s {
  /** Commence a partir de ce cylindre */
  unsigned vol_first_cylinder;
  /** 
   * Commence a partir de ce secteur 
   * dans le cylindre \a vol_first_cylinder 
   */
  unsigned vol_first_sector;
  /** Nombre de secteur dont est compose le volume */
  unsigned vol_n_sector;
  /** Type du volume */
  enum vol_type_e vol_type;
};

/**
 * \brief Structure qui decrit le Master Boot Record
 * \struct mbr_s mbr.h "mbr.h"
 */
struct mbr_s {
  /** Tableau des volumes present sur le disque */
  struct vol_s mbr_vol[MAX_VOL];
  /** Nombre de volumes sur le disque */
  unsigned mbr_n_vol;
  /** Nombre magique */
  unsigned mbr_magic;
};


/**
 * Initialise le Master Boot Record pour une première utilisation
 */
void init_mbr();

/**
 * Charge le Master Boot Record si possible.
 * \return 1 en cas de succès sinon 0
 */
int load_mbr();

/**
 * Sauvegarde, sur le premier cylindre et premier secteur, le 
 * Master Boot Record.
 * \return 1 en cas de succès sinon 0
 */
int save_mbr();

/**
 * Lit le bloc \a nbloc du volume \a vol.
 * \param[in] vol Le volume sur lequel sont les données que l'on veut 
 *                récupérer.
 * \param[in] nbloc Le numéro du bloc où sont les données que l'on veut 
 *                  récupérer, dans le volume \a vol.
 * \param[out] buffer Buffer dans lequel sont écrite les données lues.  
 */
void read_bloc(const unsigned int vol, 
	       const unsigned int nbloc, 
	       unsigned char* buffer);

/**
 * Lit, au maximum, \a n octets du bloc \a nbloc du volume \a vol .
 * \param[in] vol Le volume sur lequel sont les données que l'on veut 
 *                récupérer.
 * \param[in] nbloc Le numéro du bloc où sont les données que l'on veut 
 *                  récupérer, dans le volume \a vol.
 * \param[out] buffer Buffer dans lequel sont écrite les données lues.
 * \param[in] n Nombre d'octets.
 */
void read_bloc_n(const unsigned int vol, 
		 const unsigned int nbloc, 
		 unsigned char* buffer,
		 const size_t n);

/**
 * Ecrit le contenu de \a buffer sur le bloc \a nbloc du volume \a vol.
 * \param[in] vol Le volume sur lequel on veut ecrire les donnees de \a buffer.
 * \param[in] nbloc Le numero du bloc sur lequel on veut écrire les données 
 *                  dans le volume \a vol.
 * \param[in] buffer Buffer qui contient les donnees que l'on veut écrite sur 
 *                   le disque.
 */
void write_bloc(const unsigned int vol, 
		const unsigned int nbloc, 
	        const unsigned char* buffer);

/**
 * Ecris, au maximum, \a n octets sur le bloc \a nbloc du volume \a vol.
 * \param[in] vol Le volume sur lequel on veut ecrire les donnees de \a buffer.
 * \param[in] nbloc Le numero du bloc sur lequel on veut écrire les données 
 *                  dans le volume \a vol.
 * \param[in] buffer Buffer qui contient les donnees que l'on veut écrite sur 
 *                   le disque.
 * \param[in] n Nombre d'octets.
 */
void write_bloc_n(const unsigned int vol, 
		  const unsigned int nbloc, 
		  unsigned char* buffer,
		  const size_t n);

/**
 * Supprime les données du volume \a vol, en conservant ce volume \a vol.
 * \param[in] vol Le volume à formater.
 */
void format_vol(const unsigned int vol);


/**
 * Crée un volume à partir du couple \a cylinder et \a sector 
 * de \a nbloc bloc(s), si possible.
 * \param[in] cylinder Commence à partir de ce cylindre.
 * \param[in] sector Commence à partir de ce secteur 
 *                   dans le cylindre \a cylinder.
 * \param[in] nbloc Nombre de bloc pour le volume.
 * \return 1 en cas de succes sinon 0.
 */
int make_vol(const unsigned cylinder, 
	     const unsigned sector, 
	     const unsigned nbloc);

/**
 * Affiche les volumes présents sur le disque.
 */
void display_vol();

#endif
