import java.io.*;
import java.net.*;


public class ServerUDP {
    
    private Server server;
    
    public ServerUDP(Server server) {
        this.server = server;
    }
    
    
    // Fonction qui va envoyer le paquet UDP en fonction de ses parametres
    public boolean sendUDP(byte typeFlux, Utilisateur user) {
        try {
            //Instanciation de la Socket d'envoi et du paquet d'envoi vierge
            DatagramSocket dso=new DatagramSocket();
            byte[] dataToSend = new byte[3];
            byte[] nbFlux = user.encodeFlux();
            
            try{
            	//Creation des data a envoyer
            	dataToSend[0] = typeFlux; //Corresponds a Y
            	dataToSend[1] = nbFlux[0]; //Corresponds au premier X
            	dataToSend[2] = nbFlux[1]; //Corresponds au deuxieme X
            	
            	//Envoi du paquet avec les data (au serveur)
            	InetSocketAddress ia = new InetSocketAddress(user.getAdresse() , user.getPortUDP());
            	DatagramPacket paquet = new DatagramPacket(dataToSend, dataToSend.length, ia);
            	dso.send(paquet);
            } catch(Exception e) {
            // 	e.printStackTrace(); //DEBUG
            }
            dso.close();
        } catch(Exception e) {
            // e.printStackTrace(); //DEBUG
        }
		return false;
	}
}