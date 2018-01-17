open Jeu
open Affichage

(******************************************************)
(* Fonction qui prend une variable un état de l'automate
 et affiche la formule correspondante******************)
(******************************************************)

let formuleState s (v:formule)=match s with
    A->v
  |D->Neg (v);;


(*********************************************************************)
(* Retourne la variable associée à l'indice de la grille de taille n *)
(*********************************************************************)

let assignation i j n=
  Var (string_of_int ((i*n)+j+1));;
(******************************************************)
(* Inhibe une règle qui rend une cellule morte vivante*)
(******************************************************)
let rec inhiberD (aut:automaton) n i j=match aut with
		    |[]->[]
		    |(a,b,c,d,e)::t when (e=D)->Ou( Neg (formuleState a (assignation ((mod) ((i-1)+n) n) j n)),
						      (Ou ( Neg (formuleState b (assignation i ((mod) ((j+1)+n) n) n)),
							    (Ou ( Neg (formuleState c (assignation ((mod) ((i+1)+n) n) j n)),
								  (Ou (Neg (formuleState d (assignation i ((mod) ((j-1)+n) n) n)),Neg (formuleState e (assignation i j n)))))))))::(inhiberD t n i j)
		    |h::t->inhiberD t n i j;;

(******************************************************)
(* Inhibe une règle qui rend une cellule vivante morte*)
(******************************************************)
let rec inhiberA (aut:automaton) n i j=match aut with
		    |[]->[]
		    |(a,b,c,d,e)::t when (e=A)->Ou( Neg (formuleState a (assignation ((mod) ((i-1)+n) n) j n)),
						      (Ou ( Neg (formuleState b (assignation i ((mod) ((j+1)+n) n) n)),
							    (Ou ( Neg (formuleState c (assignation ((mod) ((i+1)+n) n) j n)),
								  (Ou (Neg (formuleState d (assignation i ((mod) ((j-1)+n) n) n)),Neg (formuleState e (assignation i j n)))))))))::(inhiberA t n i j)
		    |h::t->inhiberA t n i j;;


(**************************************************)
(*Le plus grand ensemble des règles qui peut rendre
 une cellule vivante morte*************************)
(**************************************************)

let aut=[(A,A,A,A,A);
	 (A,A,A,D,A);
	 (A,A,D,A,A);
	 (A,A,D,D,A);
	 (A,D,A,A,A);
	 (A,D,A,D,A);
	 (A,D,D,A,A);
	 (A,D,D,D,A);
	 (D,A,A,A,A);
	 (D,A,A,D,A);
	 (D,A,D,A,A);
	 (D,A,D,D,A);
	 (D,D,A,A,A);
	 (D,D,A,D,A);
	 (D,D,D,A,A);
	 (D,D,D,D,A) ];;

(*********************************************************************)
(* Fonction qui donne l'ensemble des règles rendant une cellule vivante
 morte en fonction de l'automate  ************************************)
(*********************************************************************)

let  rec autd aut a=match aut with
    []->[]
    |h::t when (List.mem h a)->autd t a
    |h::t->h::(autd t a) ;;



(*****************************************************)
(* Fonction qui associe à un automate et la dimension 
d'une grille une formule en forme normale conjonctive*)
(*****************************************************)

let stables a n:(formule list)=
  let automateD= autd aut a and l=ref []in
  for i=0 to (n-1) do
    for j=0 to (n-1) do
      let e=(inhiberD a n i j)@(inhiberA automateD n i j) in
      l:=e@(!l)
    done;
  done;!l;;



(*********************************************************************)
(* transforme une clause disjonctive en une ligne de clause disjonctive 
d'un fichier dimacs ***************************************************)

let rec formule_format_dimacs f=match f with
    Var x->x
  |Ou (x,y)->(formule_format_dimacs x)^" "^(formule_format_dimacs y)
  |Neg (Neg (x))->formule_format_dimacs x
  |Neg (x)->"-"^(formule_format_dimacs x)
  |Et(_,_) | Vrai | Faux->raise FormuleInvalide;;  

  
(*********************************************************************)
(* Creé un fichier dimacs à partir d'une liste de clauses disjonctives*)
(*********************************************************************)

let create_dimacs l nom=
  let taille=first (parse "fichier") in
  let c=open_out nom in
  let n=taille*taille in
  output_string c ("p cnf "^(string_of_int n)^" "^(string_of_int (List.length l))^"\n");
  let rec aux l=match l with
  |[]->close_out c
  |h::t->output_string c (h^" 0\n");aux t in
  aux l;;

(**************************************)
(* Lire un carractère dans un fichier *)
(**************************************)
let lireChar c=let s=
		  try
		    Some (input_char c)
		  with End_of_file->None 
		in s;;


(*********************************************************************)
(*Lire une variable ou le resultat dans le fichier de sortie de minisat*)
(*********************************************************************)

let rec lire_variable c=
  let s1=lireChar c in
  match s1 with
  |Some ' '->""
  |Some '\n'->""
  |Some x->(String.make 1 x)^(lire_variable c)
  |None->"";;


(*************************************************************************************)
(* Stocke le resultat et les variables du fichier de sortie de minisat dans une liste *)
(**************************************************************************************)

let rec string_fichier c=
  let var=lire_variable c in
  if (var="") then (close_in c;[]) else (var::(string_fichier c));;


(**************************************************************************)
(* Retourne en format dimacs la négation d'une variable en format dimacs  *)
(**************************************************************************)

let negationVariableFichier s=match (String.get s 0) with
|'-'->String.sub s 1 ((String.length s)-1)
|x->"-"^s;;


(*********************************************************************)
(*Retourne en format dimacs la négation des variables en formats dimacs*)
(*********************************************************************)

let rec negationCld l=match l with
  |[]->""
  |[h]->""
  |h::t->(negationVariableFichier h)^" "^(negationCld t);;


(***************************************************************************)
(* transforme une formule en une chaine de carractère ou chaque carractère
 correspond à l'état d'une cellule****************************************  *)

let rec generation_formule l=
  match l with
  |[]->""
  |[h]->""
  |h::t when ((String.get h 0)='-')->"D"^(generation_formule t)
  |h::t->"A"^(generation_formule t);;



(****************************************************************************)
(*Demande à l'utilisateur de taper une touche au clavier et retourne le caractère
 saisie par l'utilisateur********************************************************)
(********************************************************************************)

let saisie_stable ()=print_string "Tapez 1 pour rechercher une génération stable\n";
  print_string "Tapez un autre entier pour quitter le programme\n";
  let n=let rec aux ()=try read_int () with Failure s-> print_string "Format invalide! entrez un entier:";aux () in aux () in 
  n;;


(*********************************************************************************************)
(*****crée un ﬁchier contentant la formule caractérisant les générations stables au format DIMACS,
 lance minisat sur ce ﬁchier,récupère et parse le résultat de minisat, aﬃche la génération***
 stable correspondante (ou termine), et ﬁnalement, met à jour le contenu de la liste de clauses
 disjonctives décrite plus haut, pour éliminer la génération qui vient d’être calculée *******)
(*********************************************************************************************)

let show_stable ()=
  let t=parse "fichier" in
  let taille=first t in
  let automate=scnd t in
  let listes_formule_stables=ref [] in
  listes_formule_stables:=(List.map formule_format_dimacs (stables automate taille))@(!listes_formule_stables);		 
  let rec loop ()=
    create_dimacs (!listes_formule_stables)  "entree.dimacs";
    let _=Sys.command "minisat -verb=0 entree.dimacs sortie" in
    let c=open_in "sortie" in
    let l=string_fichier c in
    match l with
    |[]->raise InvalideFile
    |h::t when (h="SAT")->(let g=creer_grille taille in
			   let s=generation_formule t in
			   for i=0 to (taille-1) do
			     for j=0 to (taille-1) do
			       g.cell.(i).(j)<-(stateChar (String.get s ((i*taille)+j)))
			     done;
			   done;
			   (show_generation g);
			   listes_formule_stables:=(!listes_formule_stables)@[negationCld t ];if ((saisie_stable ())=1) then loop () else () )
    |h::t->(print_string "Il n y a pas de génération stable";if ((saisie_stable ())=1) then loop () else () ) in
  loop ();;
