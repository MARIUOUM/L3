open Jeu
open Affichage
open Generations_stables
open Graphics


(*****************************************************************************************)
(* Fonction qui permet de creer un ﬁchier texte nommé  contenant la spéciﬁcation
 d’un automate cellulaire et d’une génération initiale pour cet automate, *)
(*****************************************************************************************)
let ecrireFichierGraphic f=
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
  output_string c "GenerationZero\n";
  let rec loop m=
    if m=0 then ()
    else
      (output_string c ((String.make taille 'D')^"\n");loop (m-1)) in
  loop taille;
  close_out c;;

(*******************************************************************************************************)
 (* (graphique) Attend que l'utilisateur saisisse une touche au clavier ou clique sur la fenetre.********
Si cette touche est 'q' alors l'exception Exit est lever sinon reprendre l'opération.********************
Si une cellule a été toucheé par un clique de la souris alors les indices de la cellule sont retournées  *)
(********************************************************************************************************)
 let rec wait_next_move ()=
   let taille=first (parse "fichier1") in
   let etat=wait_next_event [Button_down;Key_pressed] and n=(size_x()/taille) in
   if etat.keypressed then (
     match etat.key with 
       'q'->raise Exit
     |_->wait_next_move ())
   else(
     let y=(etat.mouse_x/n) and x=((taille-(etat.mouse_y/n))-1) in
     if ((y>=taille) || (y<0) || (x>=taille) || (x<0)) then (wait_next_move ()) else (x,y));; 



(***************************************************************************************************)
 (*Etant donné un automate et une génération initiale, aﬃche dans la console l’évolution du système, en montrant
 ****les générations obtenues les unes après les autres selon un certain intervalle temporel**********)
(***************************************************************************************************)

let rec evolution g =
  let automate=scnd (parse "fichier") in
  let rec aux ()=
  show_generation g;
  Unix.sleep 1;
  show_generation (next_generation (automate,g)) in aux ();;  


(***************************************************************************************************)
 (*Etant donné un automate et une génération initiale, aﬃche graphiquement l’évolution du système, en montrant
 ****les générations obtenues les unes après les autres selon un certain intervalle temporel**********)
(***************************************************************************************************)

let rec evolution_graphic g =
  let automate=scnd (parse "fichier1") in
  let rec aux ()=
  draw_board ();
  show_graphic_generation g;
  Unix.sleep 1;
  show_graphic_generation (next_generation (automate,g)) in aux ();; 
 
(*Dessine n lignes horizontales*) 
let horizontal n=
  let rec loop m=
    if m=(n+1) then () else (
    let space =(size_y ()) / n in
    let y=space*m in
    moveto 0 y;
    lineto (size_x ()) y;
    loop (m+1) ) in
  loop 1;;

(*Dessine n lignes verticales *) 
let vertical n=
   let rec loop m=
     if m=0 then ()else (
    let space =(size_x ()) / n in
    let x=space*m in
    moveto x 0;
    lineto x (size_y ());
    loop (m-1)) in
   loop n;;

(*********************************************************************)
(*  Rentre graphiquement une génération initiale en cliquant avec 
la souris les cases d’une grille vide******************************* *)
(*********************************************************************)

 let generation_initiale_graphic ()=
   let taille=first (parse "fichier1") in
   let g=creer_grille taille in
   let rec loop ()=
     draw_board ();
     set_color black;
     vertical taille;
     horizontal taille;
     show_graphic_generation g;
     try
       let indice=wait_next_move () in
       let i=fst indice and j=snd indice in
       if (g.cell.(i).(j)=A) then (g.cell.(i).(j)<-D) else (g.cell.(i).(j)<-A);
       show_graphic_generation g;
       loop ()
     with Exit->g in
   loop ();;

(*********************************************)
(*Voir graphiquement les générations stables *)
(*********************************************)
let show_stable_graphic ()=
  let t=parse "fichier1" in
  let automate=scnd t in
  let taille=first t in
  let listes_formule_stables=ref [] in
  listes_formule_stables:=(List.map formule_format_dimacs (stables automate taille))@(!listes_formule_stables); 
  let rec loop ()=
    create_dimacs (!listes_formule_stables)  "entree.dimacs";
    let _=Sys.command "minisat -verb=0 entree.dimacs sortie" in
    let c=open_in "sortie" in
    let l=string_fichier c in
    let rec aux ()=
      let etat=wait_next_event [Key_pressed] in
	match etat.key with 
	|'r'->loop ()
	|'q'->raise Exit
	|_->aux () in 
    match l with
    |[]->raise InvalideFile
    |h::t when (h="SAT")->
      begin
	let g=creer_grille taille in
	let s=generation_formule t in
	for i=0 to (taille-1) do
	  for j=0 to (taille-1) do
	    g.cell.(i).(j)<-(stateChar (String.get s ((i*taille)+j)))
	  done;
	done;
	draw_board ();
	show_graphic_generation g;
	listes_formule_stables:=(!listes_formule_stables)@[negationCld t ];
	aux ();
      end
    |h::t->begin
      draw_board ();
      set_color white;
      fill_rect 0 0 (size_x()) 50;
      moveto 100 25;
      set_color black;
      draw_string "Il n y a pas de génération stable";
      aux ();
    end in
  loop ();;

let rec main ()=
  print_string "\n*****MENU PRINCIPAL*****\n1)mode texte \n2)mode graphique\n3)Quitter\n";
  let rec loop ()=
    let n=let rec aux ()=try read_int () with Failure("int_of_string")-> print_string "Format invalide! entrez un entier:"; aux () in aux () in
    match n with 
    |1->begin
      print_string "\n*****MODE TEXTE*****\n1)Charger un fichier \n2)Creer un nouveau fichier\n3)Retour au MENU MPRINCIPAL\n4)Quitter\n";
      let rec loop1 ()=
	let n=let rec aux ()=try read_int () with Failure s-> print_string "Format invalide! entrez un entier:"; aux () in aux () in
        if n=2 then (ecrireFichier "fichier") else if (n=3) then main () else if (n=4) then (raise Exit) else ();
	let d=parse "fichier" in
	let generation=trd d in
	print_string "1)Evolution des générations de l'automate \n2)Trouver les générations stables\n3)Retour au MENU MPRINCIPAL\n4)Quitter\n";
	let n=let rec aux ()=try read_int () with Failure s-> print_string "Format invalide! entrez un entier:"; aux () in aux () in
	match n with
	|1->print_string "\n*****Evolution des générations de l'automate*****\n"; evolution generation
	|2-> print_string "\n*****Les générations stables de l'automate*****\n";show_stable ()
	|3-> main ()
	|4-> raise Exit
	|_-> loop1 () in loop1 ()
    end
    |2->begin
      print_string "\n*****MODE GRAPHIQUE*****\n";
      ecrireFichierGraphic "fichier1";
      let generation=generation_initiale_graphic () in
      set_color white;
      fill_rect 0 0 (size_x()) 50;
      set_color black;
      moveto 10 25;
      draw_string "1)Evolution de l'automate 2)Générations stables 3)MENU PRINCIPAL 4)Quitter";
      let rec loop3 ()=
	let etat=wait_next_event [Key_pressed] in
	match etat.key with 
	|'1'->evolution_graphic generation
	|'2'->show_stable_graphic ()
	|'3'->main ()
	|'4'->raise Exit
	|_->loop3 () in loop3 ()
    end
    |3->raise Exit
    |_->loop () in try loop () with |Exit ->close_graph ();;
main ();;
