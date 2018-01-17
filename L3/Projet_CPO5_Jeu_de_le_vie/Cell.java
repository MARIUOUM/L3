public interface Cell<S extends State, N extends Enum<N>> {
	/**
	 * @return état courant de la cellule
	 */
	S getState();

	/**
	 * Changer l'état de la cellule.
	 * @param state : nouvel état de la cellule.
	 */
	void setState(S state);

	/**
	 * @param direction : direction du voisin que l'on cherche.
	 * @return référence vers la cellule voisine dans la direction désirée.
	 */
	Cell<S, N> getNeighbor(N direction);

	/**
	 * Simuler une transition, sans changer l'état de l'objet.
	 * @return le prochain état de la cellule après avoir effectué une
	 *         transition.
	 */
	S nextState();
}
