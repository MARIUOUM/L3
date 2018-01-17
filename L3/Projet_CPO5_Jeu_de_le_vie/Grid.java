public interface Grid<S extends State, N extends Enum<N>, C extends Cell<?, N>> {
	/**
	 * Exécuter une transition de l'automate cellulaire.
	 */
	void update();
	/**
	 * @return Chaîne représentant l'état de la grille, en vue de l'affichage.
	 */
	String stateAsString();
}
