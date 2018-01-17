import javax.swing.*;

public interface State {
	/**
	 * icône vide par défaut
	 */
	Icon DEFAULT_ICON = new ImageIcon();

	/**
	 * @return caractère représentant l'état
	 */
	char toChar(); 

	/**
	 * @return icône représentant l'état
	 */
	Icon toIcon();
	
}
