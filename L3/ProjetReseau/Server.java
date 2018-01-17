import java.net.*;
import java.io.*;
import java.util.*;


public class Server {
    
    private int debugOption;    //Option permettant de choisir le debug au lancement
    
    private final int listenPortClientTCP = 3539;       //Port d'écoute des clients
	private final int listenPortPromoteurTCP = 8969;    //Port d'écoute des promoteurs
	private final int limitHashtable = 100;             //Taille limite de la Hashtable
    private ServerUDP serverUDP;

    //Hashtable de la liste des clients (la clé est l'id, et la valeur et l'objet de cet Utilisateur) et de la liste des ports UDP
    private Hashtable<String, Utilisateur> listClients = new Hashtable<String, Utilisateur>();
    private Hashtable<Integer, String> listPortUDP =  new Hashtable<Integer, String>(); //(clé port valeur null)
        
    public Server(int debugOption) {
        this.debugOption = debugOption;
        this.serverUDP = new ServerUDP(this);
    }

    //Methode permettant d'afficher les DEBUG de base
    public static void debugPrint(String msgDebug) {
		System.out.println("DEBUG - " + msgDebug);
	}
	
	
	public static void main(String[] args) {

        //Recuperation de l'option debug
        int debugOption = 0;
        if (args.length == 1) {
		    debugOption = Integer.parseInt(args[0]);
		}

		//Création du serveur
        Server server = new Server(debugOption);
        
        //Lancement du serveur d'écoute TCP
        debugPrint("Serveur d'écoute TCP lancé"); //DEBUG
        try {
            ServerSocket serverSocket = new ServerSocket(server.listenPortClientTCP);
            while(true) {
                Socket socket = serverSocket.accept();
                System.out.println(); //SEPARATEUR
                debugPrint("Un nouveau client s'est connecté (Thread lancé)"); //DEBUG
                ServerTCP serverTCP = new ServerTCP(server, socket);
                Thread threadTCP = new Thread(serverTCP);
                threadTCP.start();
            }
        } catch(Exception e) {
            e.printStackTrace(); //DEBUG
        }
    }
	
	
	//------------ GETTER ET SETTER ------------ 
	public int getDebugOption() {
		return this.debugOption;
	}
	
    public ServerUDP getServerUDP() {
        return this.serverUDP;
    }

    public int getLimitHashtable() {
        return this.limitHashtable;
    }

    public Hashtable<String, Utilisateur> getListClients() {
		return this.listClients;
	}
	
	public Hashtable<Integer, String> getListPortUDP(){
	    return this.listPortUDP;
	}
    
    public Utilisateur getUser(String id) {
    	try{
    		return this.listClients.get(id);
    	}
    	catch(Exception e) {
    		e.printStackTrace();
    	}
    	System.out.println("ERREUR - Probleme lors du getUser de : " + id + "!"); //ERREUR
    	return null;
    }
    //------------------------------------------------ 
}