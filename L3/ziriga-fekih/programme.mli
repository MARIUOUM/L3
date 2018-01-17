open Jeu
open Affichage
open Generations_stables


val ecrireFichierGraphic : string -> unit
val wait_next_move : unit -> int * int
val evolution : Jeu.generation -> unit
val evolution_graphic : Jeu.generation -> unit
val horizontal : int -> unit
val vertical : int -> unit
val generation_initiale_graphic : unit -> Jeu.generation
val show_stable_graphic : unit -> 'a
val main : unit -> unit

(* Fonction qui permet de creer un ﬁchier texte nommé  contenant la spéciﬁcation
 d’un automate cellulaire et d’une génération initiale pour cet automate, *)
(* (graphique) Attend que l'utilisateur saisisse une touche au clavier ou clique sur la fenetre.********
Si cette touche est 'q' alors l'exception Exit est lever sinon reprendre l'opération.********************
Si une cellule a été toucheé par un clique de la souris alors les indices de la cellule sont retournées  *)
(*Etant donné un automate et une génération initiale, aﬃche dans la console l’évolution du système, en montrant
 ****les générations obtenues les unes après les autres selon un certain intervalle temporel**********)
(*Etant donné un automate et une génération initiale, aﬃche graphiquement l’évolution du système, en montrant
 ****les générations obtenues les unes après les autres selon un certain intervalle temporel**********)

(*Dessine n lignes horizontales*) 

(*Dessine n lignes verticales *) 
(* Rentre graphiquement une génération initiale en cliquant avec 
la souris les cases d’une grille vide******************************* *)
(*Voir graphiquement les générations stables *)
(*fonction principale*)
