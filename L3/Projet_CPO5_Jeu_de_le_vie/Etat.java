import javax.swing.*;

public enum Etat implements State {
	ZERO { public char toChar(){ return '0'; } },
	
	UN { public char toChar(){ return '1'; } };
		
	public Icon toIcon(){
		return DEFAULT_ICON;
	}
}
