ASE++
=====


## Auteur

 - VANEGUE Elliot
 - DEFLANDRE Gaëtan


## Lancement

    $ make
	$ bin/testCore
    $ bin/prodcons

## 1er étape : testCore

Main du premier exercice. Le main compte jusque 100 000 sur chaque coeur.
Il n'y a que un seul coeur qui compte à la fois grâce au lock.


## 2e étape : prodcons amélioré
- définition de liste de contexte -> ok
- création des contextes sur le core 0 -> ok
- démarage d'une boucle infini à la fin du main de prodcons et dans la fonction start de hw