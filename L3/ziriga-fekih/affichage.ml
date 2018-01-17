open Jeu
open Graphics

(************************************************)
(* Converti un état "state" en charactère "char"*)
(************************************************)
let charState (s:state)=match s with
  |A->'O'
  |D->'.';;


(***************************************************)
(*Fonction qui affiche une génération de l'automate*)
(***************************************************)  
let show_generation g =( print_string ("************************************\n"));
    let rec aux i j n =  if i =n then print_string ("************************************\n")
                         else (
    
	                  if j=n then ((print_string "\n"); aux (i+1) 0 n)
                           else (print_char (charState (g.cell.(i).(j)));aux i (j+1) n  ))
    in aux 0 0 g.n ;; 


(*************************************************************)
(* Dessiner une fenêtre graphique avec une couleur et un titre*)
(**************************************************************)

let draw_board ()=
    open_graph (" 600x600");
    set_window_title "Jeu de la vie";
    let couleur=rgb 155 155 100 in
    set_color couleur;
    fill_rect 0 0 (size_x ()) (size_y ());;


(*********************************************************************)
(*Prend l'indice d'une ligne et d'une colonne d'une case et dessine
 un cercle à la bonne place*******************************************)
(*********************************************************************)
let draw_symbol (i,j) couleur m=
    let n=(size_x()/m) in
    set_color couleur;
    fill_circle  ((j*n)+(n/2)) ((size_y())-(i*n)-(n/2)-4) ((size_x())/(2*m) );;


(****************************************************************)
(* Dessine des cercles dans chaque cellule qui a un etat vivant *)
(****************************************************************)
let show_graphic_generation g=
  let rec loop i j n=if i=n then () else
      if j=n then loop (i+1) 0 n else 
	if (g.cell.(i).(j)=A) then (draw_symbol (i,j) blue n;loop i (j+1) n) else loop i (j+1) n in
  loop 0 0 g.n;;
