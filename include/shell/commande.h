#ifndef COMMANDE_H
#define COMMANDE_H

/**
 * \file commande.h
 * \brief Gestionnaire de commande
 * \author Elliot Vanegue et Gaëtan Deflandre
 * \version 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * \brief Taille maximum d'une commande avec ses options
 */
#define MAX_ENTRY 255

/**
 * \brief Taille maximum d'une commande 
 */
#define MAX_COMMAND 10

/**
 * \brief Taille maximum d'une option 
 */
#define MAX_OPTION 255

/**
 * \brief Taille maximum d'un chemin 
 */
#define MAX_PATH 255

/**
 * \brief Position des commandes par rapport au shell 
 */
#define COMMAND_PATH ""

extern char* current_path;

#endif

