TP1 : try/catch
===============

# Auteur

- Deflandre Gaëtan
- Vanegue Elliot


# Description

## Display stack

Le programme display_stack affiche la pile des fonctions appelées.
- L'adresse du pointeur *esp* (stack pointer) est au-dessus de l'adresse du pointeur *ebp* (base pointer) dans la pile. C'est-a-dire que l'adresse *esp* est avant celle de *ebp* dans la mémoire.
- Les adresses des variables automatiques locales se trouvent entre les pointeurs *esp* et *ebp*.
- Les adresses des paramètres de la fonction courante se trouvent sous le pointeur *ebp* dans la pile. C'est-a-dire plus long dans la mémoire.
- Lorsqu'on appelle une fonction depuis une autre fonction. Les emplacements des pointeurs *esp* et *ebp* pour la fonction qui a été appelé sont avant dans la mémoire par rapport a la fonction qui a appalé. La fonction qui a été appelé est au-dessus dans la pile.


## Try/Catch

L'implémantation des fonctions try et throw permet au programme de ne pas 
refaire toutes les étapes d'une fonction récurcive s'il y a une erreur ou 
dans un cas qui n'est pas interessant pour l'algorithme.

Ces deux fonctions permettent de déplacer le pied et la tête du registre afin 
de pointer sur une précédente partie du code.
