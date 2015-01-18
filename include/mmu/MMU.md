Virtualisation de la mémoire du microprocesseur
===============================================

# Auteurs

 - Elliot VANEGUE
 - Gaëtan DEFLANDRE

# Description

Gére le mapping mémoire physique et mémoire virtuelle avec deux contextes courrants.

## src

Les fichiers sources sont:

 - mi_syscall.h
 - mi_kernel.c *les fonctions à utiliser en mode maître*
 - mi_user.c *les fonction à utiliser en mode utilisateur*
 
## exécutable

L'exécutable:

 - test
 
Utilisation:
 
    $ ./test
