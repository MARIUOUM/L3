open Jeu
open Affichage

val formuleState : Jeu.state -> Jeu.formule -> Jeu.formule
val assignation : int -> int -> int -> Jeu.formule
val inhiberD : Jeu.automaton -> int -> int -> int -> Jeu.formule list
val inhiberA : Jeu.automaton -> int -> int -> int -> Jeu.formule list
val aut : (Jeu.state * Jeu.state * Jeu.state * Jeu.state * Jeu.state) list
val autd : 'a list -> 'a list -> 'a list
val stables : Jeu.automaton -> int -> Jeu.formule list
val formule_format_dimacs : Jeu.formule -> string
val create_dimacs : string list -> string -> unit
val lireChar : in_channel -> char option
val lire_variable : in_channel -> string
val string_fichier : in_channel -> string list
val negationVariableFichier : string -> string
val negationCld : string list -> string
val generation_formule : string list -> string
val saisie_stable : unit -> int
val show_stable : unit -> unit

(* Fonction qui prend une variable un état de l'automate
 et affiche la formule correspondante*)
(* Retourne la variable associée à l'indice de la grille de taille n *)
(* Inhibe une règle qui rend une cellule morte vivante*)
(* Inhibe une règle qui rend une cellule vivante morte*)
(*Le plus grand ensemble des règles qui peut rendre
 une cellule vivante morte*************************)
(* Fonction qui donne l'ensemble des règles rendant une cellule vivante
 morte en fonction de l'automate  ************************************)
(* Fonction qui associe à un automate et la dimension 
d'une grille une formule en forme normale conjonctive*)
(* transforme une clause disjonctive en une ligne de clause disjonctive 
d'un fichier dimacs ***************************************************)
(* Creé un fichier dimacs à partir d'une liste de clauses disjonctives*)
(* Lire un carractère dans un fichier *)
(*Lire une variable ou le resultat dans le fichier de sortie de minisat*)
(* Stocke le resultat et les variables du fichier de sortie de minisat dans une liste *)
(* Retourne en format dimacs la négation d'une variable en format dimacs  *)
(* transforme une formule en une chaine de carractère ou chaque carractère
 correspond à l'état d'une cellule****************************************  *)
(*Demande à l'utilisateur de taper une touche au clavier et retourne le caractère saisie par l'utilisateur*)
(*****crée un ﬁchier contentant la formule caractérisant les générations stables au format DIMACS,
 lance minisat sur ce ﬁchier,récupère et parse le résultat de minisat, aﬃche la génération***
 stable correspondante (ou termine), et ﬁnalement, met à jour le contenu de la liste de clauses
 disjonctives décrite plus haut, pour éliminer la génération qui vient d’être calculée *******)
