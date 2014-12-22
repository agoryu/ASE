Synchronisation entre contextes
===============================

# Auteur

- Elliot Vanegue
- Gaetan Deflandre


# Description

Le programme change de contexte avec un ordonnancement sur interruption.

Dans notre exemple, le programme ne boucle plus à l'infini, mais se 
termine sur le main. Nous conservons les registres *esp* et *ebp*, 
lors du premierappel à *switch\_to\_ctx*.

Pour illustrer les interruptions, nous avons utilisés la fonction sleep.
Ce qui laisse le temps au programme de changer de contexte.

On peut maintenant, en plus de l'ordonnancement sur interruption, gérer les 
contextes grâce à un système de sémaphores. 

