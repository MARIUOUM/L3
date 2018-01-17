import java.lang.Thread;
import java.util.Scanner;

public class MainJeuDeLaVie{

	public static void main(String[] args){
		Grille g = new Grille(10,10);
		Scanner input = new Scanner(System.in);
		
		System.out.println("Jeu de la vie: Bienvenue :) !!!");
    System.out.println(
        "Choisissez une variante à exécuter: \n" +
        "  1) HighLife\n" +
        "  2) DayAndNight\n" +
        "  3) Automate Cellulaire \n" +
        "  4) Exit\n "
    );
    int choix = input.nextInt();
    input.nextLine();
    switch (choix) {
    case 1:
      g.initGrille(HighLife.class);
			System.out.println(g.stateAsString());
      break;
    case 2:
      g.initGrille(DayAndNight.class);
			System.out.println(g.stateAsString());
      break;
    case 3:
      g.initGrille(AutomateCellulaire.class);
			System.out.println(g.stateAsString());
      break;
    default:
      System.out.println("Ciao!");
      System.exit(0);
      break;
    }
		
		while(true){
			g.update();
			System.out.println(g.stateAsString());
			try{
				Thread.sleep(300);
			} catch(Exception e){
				System.out.println(e);
			}
		}
	}
}
