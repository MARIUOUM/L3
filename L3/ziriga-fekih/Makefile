

#Edition des liens et creation de l'executable
jeu-de-la-vie.exe: jeu.cmo affichage.cmo generations_stables.cmo programme.cmo
	ocamlc  -custom graphics.cma unix.cma -o jeu-de-la-vie.exe jeu.cmo affichage.cmo generations_stables.cmo programme.cmo

#Compilation du corps du module jeu
jeu.cmo: jeu.ml jeu.cmi
	ocamlc -c jeu.ml

#Compilation de l'interface du module jeu
jeu.cmi:jeu.mli 
	ocamlc  jeu.mli 
	

#Compilation du corps du module affichage
affichage.cmo: affichage.ml jeu.cmi affichage.cmi
	ocamlc -c affichage.ml
	
#Compilation de l'interface du module jeu
affichage.cmi:affichage.mli 
	ocamlc  affichage.mli 
	

#Compilation du corps du module de générations stables	
generations_stables.cmo: generations_stables.ml jeu.cmi affichage.cmi generations_stables.cmi
	ocamlc -c generations_stables.ml
	
#Compilation de l'interface du module jeu
generations_stables.cmi:generations_stables.mli 
	ocamlc  generations_stables.mli 

#Compilation du corps du module programme	
programme.cmo: programme.ml jeu.cmi affichage.cmi generations_stables.cmi programme.cmi
	ocamlc -c programme.ml
	
#Compilation de l'interface du module jeu
programme.cmi:programme.mli 
	ocamlc  programme.mli 
	
	


#Effacer fichiers auxiliaires
clean:
	rm  *.cmi  *.cmo
