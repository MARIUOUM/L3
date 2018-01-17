type state = A | D(* Les états possibles d'une cellule A:vivant ,D:mort*)
type generation = { n : int; cell : state array array; }(*Une génération:un ensemble fini de cellules vivantes*)
type rule = state * state * state * state * state(*Une règle de l'automate: Elle dit si une cellulevit ou pas en fonction de son voisinage*)
type automaton = rule list(* Un automate:Ensemble de règles faisant évoluer une génération*)
(* type pour representer Les formules la logique propositionnelle *)
type formule =
    Vrai
  | Faux
  | Var of string
  | Neg of formule
  | Et of formule * formule
  | Ou of formule * formule
exception StateForbidden;;(*Exception pour un charactère qui n'est pas un état possible de la cellule*)
exception InvalideFile;;(*Exception pour un fichier invalide*)
exception FormuleInvalide;;(*Exception pour une formule invalide*)exception StateForbidden

val stateChar : char -> state (* Convertir un char en state *)
val ruleString : string -> rule(*Convertir un string en un rule *)
val lit_regles : in_channel -> automaton -> automaton(*Lecture des règles du fichier *)
val creer_grille : int -> generation(*création de grille avec toutes les cellules mortes *)

(*****************************************************************************************)
(* Fonction qui permet de creer un ﬁchier texte nommé  contenant la spéciﬁcation
 d’un automate cellulaire et d’une génération initiale pour cet automate, *)
(*****************************************************************************************)
val ecrireFichier :string -> unit


(**********************************************************************************)	   
(*lit un fichier texte contenant la dimension d'une grille,la spécification 
et une génération initiale d'un automate cellulaire et renvoie ces données .Si
le fichier est vide ou n'existe pas ou contient un caractère invalide elle demande à l'utilisateur de creer un nouveau fichier*)
(**********************************************************************************)
val parse : string -> int  *  automaton  *  generation

(***************************************)
(*Recupération des élément d'un triplet*)
(***************************************)
val first :'a * 'b * 'c -> 'a
val scnd :'a * 'b * 'c -> 'b
val trd :'a * 'b * 'c -> 'c


(*******************************************)
(* Calcule de la prochaine génération ******)
(*******************************************)
val next_generation : automaton * generation -> generation

