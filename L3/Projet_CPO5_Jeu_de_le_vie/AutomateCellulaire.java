import java.util.*;

/*Classe héritant d'une cellule*/
public class AutomateCellulaire extends Cellule{
	
	/* Constructeur AutomateCellulaire */
	public AutomateCellulaire(){
		super();
		this.setState(LifeState.values()[new Random().nextInt(LifeState.values().length)]);
	}
	
	/* Redefinittion de la methode nextState
	 * @return le prochain etat de la cellule: LifeState
	 */
	public LifeState nextState(){
		int nb = numberAliveNeighbor();
		if( first_rule(nb) || second_rule(nb) ){
			return LifeState.ALIVE;
		} 
		return LifeState.DEAD;
	}
	
	/* La première règle du jeu:
	 * Une cellule vivante et dont deux ou trois voisins sont vivants reste vivante sinon elle meurt.
	 * @param: n: un entier représentant le nombre voisins vivants.
	 * @return: un booleen vrai si la règle est appliquée ou faux sinon
	 */
	public boolean first_rule(int n){
		if( this.s == LifeState.ALIVE && (n == 2 || n == 3) ){
			return true;
		}
		return false;
	}
	
	/* La deuxième règle du jeu:
	 * Une cellule morte et dont exactement trois voisins sont vivants nait ou reste mort sinon.
	 * @param: n: un entier représentant le nombre voisins vivants.
	 * @return: un booleen vrai si la règle est appliquée ou faux sinon
	 */
	public boolean second_rule(int n){
		if( this.s == LifeState.DEAD && n == 3 ){
			return true;
		}
		return false;
	}
	
	/* Compte le nombre de voisins vivant de la cellule
	 * @return:  un entier représentant le nombre voisins vivants.
	 */
	public int numberAliveNeighbor(){
		int compteur = 0;
		Set<SquareGridNbh> st = cellsNeighbors.keySet();
		Iterator<SquareGridNbh> it = st.iterator();
		while(it.hasNext()){
			SquareGridNbh key = it.next();
			Cellule c = cellsNeighbors.get(key);
			if( (c != null) && (c.s == LifeState.ALIVE) ){
				compteur++;
			}
		}
		return compteur;
	}
}
