import javax.swing.*;

public enum LifeState implements State{
	DEAD{ public char toChar(){ return '.';} }, ALIVE{ public char toChar(){ return '0';} };
	
	public Icon toIcon(){
		return DEFAULT_ICON;
	}	
}
