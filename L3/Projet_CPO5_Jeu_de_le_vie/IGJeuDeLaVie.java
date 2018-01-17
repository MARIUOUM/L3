import javax.swing.*;
import java.awt.*;
import java.lang.Thread;
import java.awt.event.*;

public class IGJeuDeLaVie extends Thread implements ActionListener {

	private final int LARGEUR = 700;
	private final int HAUTEUR = 700;
	
	private final int ROWS = 70;
	private final int COLUMNS = 70;
	
	private JFrame window;
	private JButton execute;
	private JPanel panel;
	private JPanel pan;
	private JLabel[] labels;
	private Grille grille;
	private boolean start = false;
	public Thread t;
	
	public IGJeuDeLaVie(){
		window = new JFrame("Fenêtre Jeu de la vie");
		window.setPreferredSize(new Dimension(LARGEUR,HAUTEUR));
		
		grille = new Grille(ROWS,COLUMNS);
		grille.initGrille(DayAndNight.class);
		
		panel = new JPanel();
		
		pan = new JPanel(new GridLayout(ROWS,COLUMNS,2,2));
		labels = new JLabel[ROWS * COLUMNS];
		dessiner();
		t = new Thread("Game");
		
		pan.setBorder(BorderFactory.createLineBorder(Color.BLACK));
		execute = new JButton("Run!");
		execute.addActionListener(this);
		
		
		panel.add(execute);
		window.add(pan);
		window.add(panel,BorderLayout.SOUTH);
		window.pack();
		window.setLocationRelativeTo(null);
		window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		window.setVisible(true);
	}
	
	public void dessiner(){
		int k=0;
		for(int i=0; i<ROWS; i++){
			for(int j=0; j<COLUMNS; j++){
				labels[k] = new JLabel();
				if(grille.cells[i][j].getState() == LifeState.ALIVE){
					labels[k].setBackground(Color.BLACK);
					labels[k].setOpaque(true);
				}
				pan.add(labels[k++]);
			}
		}
	}
	
	public void actionPerformed(ActionEvent e){
		System.out.println("OK!");
		start = !start;
		if(start){
			this.start();
		}
		else {
			try{
			this.interrupt();
			this.join();
			}catch(Exception ex){
			
			}
		}
	}
	
	public void run(){
		System.out.println(start);
		while(start){
			grille.update();
			pan.removeAll();
			dessiner();
			pan.revalidate();
			pan.repaint();
			try{
					Thread.sleep(90);
			} catch(Exception ex){
					Thread.currentThread().interrupt();
					System.out.println(ex);
			}
		}
	}
		
}
