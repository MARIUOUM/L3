(* ************************************************************ 
		        Projet Ocaml
 *************************************************************
   Auteur :	Jules Camille Ziriga  
                Meriem Fekih Ahmed
 Fichiers :	jeu.ml affichage.ml programme.ml interface.ml
 Version :	1
 Date :		29/12/15 09:07:44
 *************************************************************
 Description :	Von Neumann de rayon 1
 ************************************************************ *)


(* Définition  des types *)

(*****************************************************)
(* Les états possibles d'une cellule A:vivant ,D:mort*)
(*****************************************************)
type state = A | D;;

(*****************************************************)
(*Une génération:un ensemble fini de cellules vivantes*)
(*****************************************************)

type generation = {n:int; cell:state array array};;

(*****************************************************)
(*Une règle de l'automate: Elle dit si une cellule 
vit ou pas en fonction de son voisinage **************)

type rule =state*state*state*state*state;;

(****************************************************************)
(* Un automate:Ensemble de règles faisant évoluer une génération*)
(****************************************************************)

type automaton =rule list;;

(****************************************************************)
(* type pour representer Les formules la logique propositionnelle *)
(****************************************************************)

type formule=Vrai|Faux
	     |Var of string
	     |Neg of formule
	     |Et of formule * formule
	     |Ou of formule * formule;;


(*****************************)
(* Définition des exceptions *)
(*****************************)

exception StateForbidden;;(*Exception pour un charactère qui n'est pas un état possible de la cellule*)
exception InvalideFile;;(*Exception pour un fichier invalide*)
exception FormuleInvalide;;(*Exception pour une formule invalide*)

(*****************************************************************************************)
(* Fonction qui permet de creer un ﬁchier texte nommé  contenant la spéciﬁcation
 d’un automate cellulaire et d’une génération initiale pour cet automate, *)
(*****************************************************************************************)
let ecrireFichier f=
  let c=open_out f in
  print_string "Taille de la grille:";
  let rec saisieEntier n=
    let t=let rec aux ()=try read_int () with Failure s-> print_string "Format invalide! entrez un entier:";aux () in aux () in
    try
      begin
	if t>n then raise Exit
	else print_endline ("La valeur saisie  doit etre un entier superieur à "^(string_of_int n)^"! recommencez!"); saisieEntier n
      end 
    with Exit->t in
  let taille=saisieEntier 1 in
  output_string c ((string_of_int taille)^"\n");
  print_endline "Regles";
  output_string c "Regles\n";
  print_endline "Nombre de règles";
  let nbreRegles=saisieEntier 0 and tailleRegle=5 in
  let rec saiseString i n tailleString= if i=n+1 then ()  
			else
			  (print_string ((string_of_int i)^" : ");
			   let s=read_line () in
			   if((String.length s)=tailleString) then (output_string c (s^"\n");saiseString (i+1) n tailleString) 
			   else (print_string "format incorrecte!! recommencez!\n";saiseString i n tailleString))
  in
  saiseString 1 nbreRegles tailleRegle;
  print_endline "Generation initiale";
  output_string c "GenerationZero\n";
  saiseString 1 taille taille;
  close_out c;;


(*****************************)
(* Convertir un char en state *)
(*****************************)

let stateChar c=match c with 
  |'A'->A
  |'D'->D
  |_->raise StateForbidden;;


(*****************************)
(*Convertir un string en un rule *)
(*****************************)

let ruleString s=((stateChar s.[0],stateChar s.[1],stateChar s.[2],stateChar s.[3],stateChar s.[4]):rule);;


(*****************************)
(*Lecture des règles du fichier *)
(*****************************)

let rec lit_regles c (l_regle:automaton)=
    let regle=(input_line c) in
    if (not(regle="GenerationZero")) then lit_regles c (ruleString regle::l_regle) else l_regle;;


(*****************************************************)
(* remplace les états d'une grille donnée en entrée par
 celles lues dans un canal donnée en entrée et retourne
 la grille modifiée **********************************)

let generation_initiale c (g:generation) =
  let rec aux i j n str = if i=n then g
                      else 
			if j=n  then  aux (i+1) 0 n (try (input_line c) with End_of_file-> "")
			else
			 ( g.cell.(i).(j)<-(stateChar str.[j]); aux i (j+1)  n str)
  in aux 0 0 g.n (input_line c) ;;



(*****************************************************)
(*création de grille avec toutes les cellules mortes *)
(*****************************************************)

let creer_grille taille={n=taille; cell=Array.make_matrix taille taille D };;

(**********************************************************************************)	   
(*lit un fichier texte contenant la dimension d'une grille,la spécification 
et une génération initiale d'un automate cellulaire et renvoie ces données .Si
le fichier est vide ou n'existe pas ou contient un caractère invalide elle demande à l'utilisateur de creer un nouveau fichier*)
(**********************************************************************************)
let parse s=
  let aux ()=
  let c=open_in s in
  let taille= (input_line c) and
      _=(input_line c) and
      regle1=(input_line c) in
  let n=int_of_string taille and regles=lit_regles c [ruleString regle1] in
  let  generation=generation_initiale c (creer_grille n) in
  close_in c;(n,regles,generation) in
  try aux ()
  with |End_of_file | Sys_error _ |StateForbidden ->print_string "Vous devez absolument creer un nouveau fichier pour continuer!!\n";ecrireFichier s;aux ();; 

(***************************************)
(*Recupération des élément d'un triplet*)
(***************************************)
let first (x,y,z)=x;;
let scnd (x,y,z)=y;;
let trd (x,y,z)=z;;


(*************************************************************)
(*Fonction qui retourne une règle correspondant aux voisinages
 de l'indice d'une cellule d'une grille entrée en paramètre***************)
(*************************************************************)
let voisinages g i j:rule=
    (g.cell.((mod) ((i-1)+g.n) g.n).(j),g.cell.(i).((mod) ((j+1)+g.n) g.n),g.cell.((mod) ((i+1)+g.n) g.n).(j),g.cell.(i).((mod) ((j-1)+g.n) g.n),g.cell.(i).(j) );;


(***************************************)
(* copier une grille ******)
(***************************************)

   let copy_grille g=
    let g'=creer_grille g.n in
    let rec aux i j n  =if i=n then g'
      else 
	if j=n then aux (i+1) 0 n
	else (g'.cell.(i).(j)<-g.cell.(i).(j); aux i (j+1)  n )
      
    in aux 0 0 g'.n ;;


(*******************************************)
(* Calcule de la prochaine génération ******)
(*******************************************)

  let next_generation ((a:automaton),g)=
    let g'=copy_grille g in
    let rec aux i j g2 =
        if i=g2.n then g2
	else
        if j=g2.n then aux (i+1) 0 g2 	
         else   (if (List.mem (voisinages g i j) a) then g2.cell.(i).(j)<-A else  g2.cell.(i).(j)<-D ;  aux i (j+1) g2) 
 in aux 0 0 g';;

