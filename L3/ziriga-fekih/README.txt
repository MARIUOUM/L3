********************************************************************************
Automate Cellulaire Von Neumann de rayon 1 v.0.1, copyleft Jules Camille Ziriga 
																													&& Meriem Fekih Ahmed
Mardi 29 decembre 2015, 8h:00
********************************************************************************



********************************************************************************
                               PRESENTATION
********************************************************************************

Le but de ce projet est d'écrire un programme OCaml qui permet de calculer et vi-
sualiser l’évolution d’un automate cellulaire, et de calculer et visualiser ses 
générations
stables. 



================================================================================

********************************************************************************
																COMPILATION
********************************************************************************

================================================================================

Ce programme utilise  un Makefile, il suffit normalement
de taper :

**make:Un exécutable est créé, appelé jeu-de-la-vie.exe



================================================================================

********************************************************************************
																UTILISATION
********************************************************************************

================================================================================

Tapez simplement

./jeu-de-la-vie.exe

Faire un Choix selon le mode affiché à l'écran:

1)mode texte 

2)mode graphique **Vous pouvez aussi lancer l’interface graphique du jeu de la vie. 
Pour exécuter le jeu, il sera nécessaire d’appuyer sur 2 **

3)Quitter

et suivez les étapes indiquées ligne par ligne et regardez l'évolution des 
générations  et l'affichage des générations stables par le biais d'un minisat qui
affichera à chaque fois si le résultat est SATISFAISABLE ou bien INSATISFAISABLE.


===================================================================================

***********************************************************************************
                             			EXPILCATIONS
***********************************************************************************
Attention!

-Si vous remplissez le fichier du programme "fichier" avec des données erronées, 
vous ne pouvez pas exécuter le programme et il affiche un message d'erreur!!
Vous ne pouvez plus exécuter le programme une autre fois.

-Si le fichier est vide ou n'existe pas, le programme demande de créer un fichier.

 ================================================================================

********************************************************************************
																Mode Graphique
********************************************************************************

================================================================================
Pour exécuter le programme en mode graphique:
En choisissant le mode graphique du menu qui s'est apparait dans le terminal, 
une grille qui s'affiche.Il est nécessaire de cliquer sur une case pour dessiner 
un cercle ou bien l'enlever. 
En cliquant sur la touche "q" du clavier, tu auras comme résultat la grille que tu 
as dessiné.
L'étape suivante :cliquer sur la touche 1 pour voir l'évolution de la grille sinon 
cliquer sur la touche 2 pour voir les générations stables. 

 
===================================================================================



********************************************************************************
                             LES FICHIERS SOURCES
********************************************************************************

===================================================================================

*Un fichier jeu.ml est fournit: pour regrouper les types et les fonctions définissant 
le jeu. 
*un fichier jeu.mli est fournit: pour faire un résumé sur les définitions du module Jeu.
*Un fichier affichage.ml est fournit: pour regrouper les fonctions qui s’occupent de 
l’affichage à l’écran et de créer l'interface grqphique.
*Un fichier generations_stables.ml:pour regrouper les fonctions qui permet de calculer 
et visualiser l'évolution d'un automate cellulaire ,et de calculer et visulaiser 
les générations stables.
*Un fichier programme.ml:qui regroupe les fonctions intéragissant avec l’utilisateur.



