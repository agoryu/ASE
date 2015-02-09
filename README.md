ASE++
=====


## Auteur

 - VANEGUE Elliot
 - DEFLANDRE Gaëtan


## Lancement

    $ make
	$ ./bin/mkvol -s 32767 -fc 0 -fs 1
	$ ./bin/mknfs
	$ ./bin/shell


## Avancement

La création du système de fichiers avec mknfs semble fonctonner correctement.
Le shell fonctionne et lance les commandes avec des arguments.
Les commandes ne fonctionnent pas toutes correctement, la commande mkdir
pause problème, on ne retrouve plus le mbr après son utilisation.


## Commandes implémentées

 - ls
 - cat [filename]
 - cd [dirname]
 - mkdir [dirname]
