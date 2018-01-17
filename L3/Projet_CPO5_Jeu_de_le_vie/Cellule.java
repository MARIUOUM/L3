import java.util.*;
import java.lang.Enum;

/* Classe Cellule abstraite représentant une Cellule */
public abstract class Cellule implements Cell<State, SquareGridNbh>{
		
	protected State s;
	private int id;
	public Map<SquareGridNbh, Cellule> cellsNeighbors;
	
	public Cellule(){
		this.cellsNeighbors = new HashMap<SquareGridNbh, Cellule>();
	}
	
	public Cellule(State etat){
		this.s = etat;
		
	}
	
	public State getState(){
		return this.s;
	}
	
	public void setState(State s){
		this.s = s;
	}
	
	public Cellule getNeighbor(SquareGridNbh direction){
		return cellsNeighbors.get(direction);
	}
	
	//public abstract Cellule getInstance();

	/*public State nextState(){
		return s;
	}*/
	
	
	/*public LifeState nextState(){
		int nb = numberAliveNeighbor();
		if( first_rule(nb) || second_rule(nb) ){
			return LifeState.ALIVE;
		} 
		return LifeState.DEAD;
	}
	
	public boolean first_rule(int n){
		if( this.s == LifeState.ALIVE && (n == 2 || n == 3) ){
			return true;
		}
		return false;
	}
	
	public boolean second_rule(int n){
		if( this.s == LifeState.DEAD && n == 3 ){
			return true;
		}
		return false;
	}
	
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
	}*/
}
