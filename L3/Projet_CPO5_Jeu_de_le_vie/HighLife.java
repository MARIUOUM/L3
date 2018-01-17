import java.util.*;

public class HighLife extends Cellule{
		
	public HighLife(){
		super();
		this.setState(Etat.values()[new Random().nextInt(Etat.values().length)]);
	}
	
	
	public State nextState(){
		int nb = numberAliveNeighbor();
		if( first_rule(nb) || second_rule(nb) ){
			return Etat.UN;
		} 
		return Etat.ZERO;
	}
	
	public boolean first_rule(int n){
		if( this.s == Etat.ZERO && (n == 3 || n == 6) ){
			return true;
		}
		return false;
	}
	
	public boolean second_rule(int n){
		if( this.s == Etat.UN && (n == 2 || n == 3)){
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
			if( (c != null) && (c.s == Etat.UN) ){
				compteur++;
			}
		}
		return compteur;
	}
}
