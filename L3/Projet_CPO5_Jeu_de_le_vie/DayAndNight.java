import java.util.*;

public class DayAndNight extends Cellule{

	public DayAndNight(){
		super();
		this.setState(LifeState.values()[new Random().nextInt(LifeState.values().length)]);
	}
	
	public LifeState nextState(){
		int nb = numberAliveNeighbor();
		if( first_rule(nb) || second_rule(nb) ){
			return LifeState.ALIVE;
		} 
		return LifeState.DEAD;
	}
	
	public boolean first_rule(int n){
		if( this.s == LifeState.ALIVE && (n == 3 || n == 4 || n == 6 || n == 7 || n == 8) ){
			return true;
		}
		return false;
	}
	
	public boolean second_rule(int n){
		if( this.s == LifeState.DEAD && (n == 3 || n == 6 || n == 7 || n==8) ){
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
	}
}
