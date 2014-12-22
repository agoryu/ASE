#ifndef _DRIVE_H_
#define _DRIVE_H_


/**
 * \file drive.h
 * \brief Accès materiel.
 * \author Elliot Vanegue et Gaëtan Deflandre
 * \version 1.1
 */


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "hardware.h"


/** Nom du fichier d'initialisation du materiel */
#define HW_CONFIG "hardware.ini"

#define ENABLE_HDA      1 
#define HDA_FILENAME    "vdiskA.bin"
#define HDA_CMDREG      0x3F6
#define HDA_DATAREGS    0x110
#define HDA_IRQ         14
#define HDA_MAXCYLINDER 16
#define HDA_MAXSECTOR   16
#define HDA_SECTORSIZE  256
#define HDA_STPS        2
#define HDA_STPC        1
#define HDA_PON_DELAY   30
#define HDA_POFF_DELAY  30 


/**
 * Lit le secteur au cylindre \a cylinder, secteur \a sector. 
 * Les données lues sont écrite dans \a buffer.
 * \param[in] cylinder Numéro du cylindre.
 * \param[in] sector Numéro du secteur.
 * \param[out] buffer Tampon où sont écrites les données lues. 
 */
void read_sector(const unsigned int cylinder, 
		 const unsigned int sector, 
		 unsigned char* buffer);

/**
 * Lit au maximum \a n octets du secteur au cylindre \a cylinder, 
 * secteur \a sector. Les données lues sont écrite dans \a buffer.
 * \param[in] cylinder Numéro du cylindre.
 * \param[in] sector Numéro du secteur.
 * \param[out] buffer Tampon où sont écrites les données lues.
 * \param[in] n Nombre d'octets.
 */
void read_sector_n(const unsigned int cylinder, 
		   const unsigned int sector, 
		   unsigned char* buffer, 
		   const size_t n);

/**
 * Ecrit le contenu de \a buffer dans le secteur au cylindre 
 * \a cylinder et secteur \a sector.
 * \param[in] cylinder Numéro du cylindre.
 * \param[in] sector Numéro du sector.
 * \param[in] buffer Contenu à écrire sur le secteur.
 */
void write_sector(const unsigned int cylinder, 
		  const unsigned int sector, 
		  const unsigned char* buffer);


/**
 * Ecrit au maximum \n octets du contenu \a buffer dans le 
 * secteur au cylindre \a cylinder et secteur \a sector.
 * \param[in] cylinder Numéro du cylindre.
 * \param[in] sector Numéro du sector.
 * \param[in] buffer Contenu à écrire sur le secteur.
 * \param[in] n Nombre d'octets.
 */
void write_sector_n(const unsigned int cylinder, 
		    const unsigned int sector, 
		    const unsigned char* buffer,
		    const size_t n);

/**
 * Ecrase les données des \a nsector secteur(s) à partir du 
 * cylindre \a cylinder et secteur \a sector, par la valeur \a value.
 * \param[in] cylinder Numéro de cylindre du premier secteur.
 * \param[in] sector Numéro du secteur.
 * \param[in] nsector Nombre de secteur à ecraser.
 * \param[in] value Valeur à écrire sur les secteurs.
 */
void format_sector(const unsigned int cylinder, 
		   const unsigned int sector, 
		   const unsigned int nsector, 
		   const unsigned int value);

/**
 * Ecrase tous les secteurs du disque par \a value.
  * \param[in] value Valeur à écrire sur les secteurs.
 */
void format_all(const unsigned int value);

#endif
