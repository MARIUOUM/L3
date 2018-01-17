import java.util.Random;
import java.lang.Enum;

public class Grille implements Grid<LifeState, SquareGridNbh, Cellule>{
	public int rows, columns;
	public Cellule[][] cells;
	private Cellule[][] tmpCells;
	
	public Grille(int n, int m){
		this.rows = n;
		this.columns = m;
		this.cells =  new Cellule[n][m];
	}
	
	public void initGrille(Class<? extends Cellule> c){
		for(int i=0; i<this.rows; i++){
			for(int j=0; j<this.columns; j++){
				try{
					this.cells[i][j] = c.newInstance();
				} catch(InstantiationException e){
				
				} catch (IllegalAccessException ex) {
            
        }
        
			}
		}
		/* Setting Neighbors */
		for(int i=0; i<this.rows; i++){
			for(int j=0; j<this.columns; j++){
				for(SquareGridNbh neighbor: SquareGridNbh.values()){
					this.cells[i][j].cellsNeighbors.put(neighbor, getNeighborCell(i,j,neighbor));
				}
			}
		}
	}
	
	public Cellule getNeighborCell(int i, int j, SquareGridNbh direction){
		switch(direction){
			
			case NORTH 			: if( i == 0 ){
									 				return this.cells[this.rows-1][j];
									 			} else {
									 				return this.cells[i-1][j];
									 			}
									 			//break;
			
			case NORTH_EAST :	if( i == 0 && j < this.columns-1 ){ // 1ere ligne et de la 1ere colonne à l'avant derniere colonne
													return this.cells[this.rows-1][j+1];
												} else if( i==0 && j == this.columns-1 ){ // 1ere ligne et derniere colonne
													return this.cells[this.rows-1][j-j];
												} else if( i > 0 && j == this.columns-1 ){ // Toutes les lignes sauf 1ere et derniere colonne
													return this.cells[i-1][j-j];
												} else {
													return this.cells[i-1][j+1];
												}
												//break;
			
			case EAST 			:	if( j == this.columns-1 ){
									 				return this.cells[i][j-j];
									 			} else {
									 				return this.cells[i][j+1];
									 			}
									 			//break;
			
			case SOUTH_EAST :	if( i == this.rows-1 && j < this.columns-1 ){
													return this.cells[i-i][j+1];
												} else if( i==this.rows-1 && j == this.columns-1 ){
													return this.cells[i-i][j-j];
												} else if( i < this.rows-1 && j == this.columns-1 ){
													return this.cells[i+1][j-j];
												} else {
													return this.cells[i+1][j+1];
												}
												//break;
			
			case SOUTH 			:	if( i == this.columns-1 ){
									 				return this.cells[i-i][j];
									 			} else {
									 				return this.cells[i+1][j];
									 			}
									 			//break;
			
			case SOUTH_WEST :	if( i == this.rows-1 && j > 0 ){
													return this.cells[i-i][j-1];
												} else if( i == this.rows-1 && j == 0 ){
													return this.cells[i-i][this.columns-1];
												} else if( i < this.rows-1 && j == 0 ){
													return this.cells[i+1][this.columns-1];
												} else {
													return this.cells[i+1][j-1];
												}
												//break;
			
			case WEST 			:	if( j == 0 ){
									 				return this.cells[i][this.columns-1];
									 			} else {
									 				return this.cells[i][j-1];
									 			}
									 			//break;
			
			case NORTH_WEST :	if( i == 0 && j > 0 ){
													return this.cells[this.rows-1][j-1];
												} else if( i == 0 && j == 0 ){
													return this.cells[this.rows-1][this.columns-1];
												} else if( i >0 && j == 0 ){
													return this.cells[i-1][this.columns-1];
												} else {
													return this.cells[i-1][j-1];
												}
												//break;
			
		}
		return null;
	}
	
	public void update(){
		//@SuppressWarnings("unchecked")
		this.tmpCells = new Cellule[this.rows][this.columns];
		for(int i=0; i<this.rows; i++){
			for(int j=0; j<this.columns; j++){
				this.tmpCells[i][j] = this.cells[i][j];
			}
		}
		
		for(int i=0; i<this.rows; i++){
			for(int j=0; j<this.columns; j++){
				this.cells[i][j].setState( this.tmpCells[i][j].nextState() );
			}
		}
	}
	
	public String stateAsString(){
		String strState = "";
		for(int i=0; i<this.rows; i++){
			strState += "|";
			for(int j=0; j<this.columns; j++){
				strState += " "+ (this.cells[i][j].getState().toChar()) +" |";
			}
			strState += "\n";
		}
		return strState;
	}
}
