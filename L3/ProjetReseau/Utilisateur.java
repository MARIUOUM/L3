import java.util.*;

public class Utilisateur{
    private String id;
    private int mdp;
    private int portUDP;
    private String adresse;
    private int flux; //Nombre de flux de l'utilisateur
    
    private HashSet<String> listAmis;	//HashSet d'id des amis
    private HashSet<Consu> listConsu;	//HashSet de consultation
    
	public Utilisateur(String _id, int _mdp, int _portUDP, String _adresse) {
        this.id = _id;
        this.mdp = _mdp;
        this.portUDP = _portUDP;
        this.adresse = _adresse;
        this.listAmis = new HashSet<String>();
        this.listConsu = new HashSet<Consu>();
        this.flux = 0;
    }

	public String toString() {
		return "(" + id + "," + mdp + "," + portUDP + "," + adresse + ")";
	}

	//Ajoute un message a la liste des message a consulter
	public boolean addConsu(Consu consu){
		try{
			this.listConsu.add(consu);
			return true;
		}catch(Exception e){
			System.out.println("ERREUR - Echec de l'ajout de la consultation au flux!"); //ERREUR
		}
		return false;
	}
	
	//Recupere un message de la liste de message a consulter et le supprime
	public Consu getConsu(){
		decrementeFlux();
		Object consu = null;
		
		Iterator it = this.listConsu.iterator();
		if(it.hasNext()) {
			consu = it.next();
		} else {
			System.out.println("ERREUR - " + this.id + " n'a pas de consu!"); //ERREUR
			return null;
		}
		this.listConsu.remove(consu);
		return (Consu)consu;
	}	

	//Ajoute un id d'utilisateur à la liste d'amis s'il n'est pas deja present
	public boolean addAmi(String id) {
		if(this.listAmis.contains(id)) {
			System.out.println("ERREUR - " + this.id + " est deja ami avec " + id); //ERREUR
			return false;
		} else {
			this.listAmis.add(id);
			return true;
		}
	}
	
	public void incrementeFlux(){
		this.flux++;
	}

	public void decrementeFlux(){
		if(this.flux>0){
			this.flux--;
		}
	}
	
    public byte[] encodeFlux() {
    	incrementeFlux();
    	byte[] fluxByte = new byte[2];
    	int premierX = this.flux%256;
    	int deuxiemeX = this.flux/256;
    	fluxByte[0] = (byte)premierX;
    	fluxByte[1] = (byte)deuxiemeX;
		return fluxByte;
    }
    
    
    //------------ GETTER ET SETTER ------------ 
	public int getFlux() {
		return this.flux;
	}

	public HashSet<Consu> getListConsu() {
		return this.listConsu;
	}

	public HashSet<String> getListAmis() {
		return listAmis;
	}

	public void setListAmis(HashSet<String> listAmis) {
		this.listAmis = listAmis;
	}
	
	public boolean isAmi(String id){
		if(this.listAmis.contains(id)) {
			return true;
		}
		else return false;
	}		

	public String getId() {
		return this.id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public int getMdp() {
		return this.mdp;
	}

	public void setMdp(int mdp) {
		this.mdp = mdp;
	}

	public int getPortUDP() {
		return this.portUDP;
	}

	public void setPortUDP(int portUDP) {
		this.portUDP = portUDP;
	}

	public String getAdresse() {
		return this.adresse;
	}

	public void setAdresse(String adresse) {
		this.adresse = adresse;
	}
	//------------------------------------------------ 
}