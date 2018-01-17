import java.io.*;
import java.net.*;

public class ClientUDP implements Runnable {

	private Client client; //Client Parent
	private final int lengthData = 3; //Taille en octet du paquet UDP à recevoir
	DatagramSocket dso;

	public ClientUDP(Client client, DatagramSocket dso) {
		this.client = client;
		this.dso = dso;
	}

	//Methode permettant d'afficher les DEBUG de base
	public void debugPrint(String msgDebug) {
		if (this.client.getDebugOption() > 0) {
			System.out.println("\nDEBUG - UDP - " + msgDebug);
		}
	}
	
	//Methode permettant d'afficher les DEBUG de réception de messages du Serveur
	public void debugPrintMsgReceived(String msgReceived) {
		if (this.client.getDebugOption() > 1) {
			System.out.println("\nDEBUG - UDP - CLIENT RECEIVED - " + msgReceived);
		}
	}


	//Fonction qui decode l'entier recu sur 2 octets
	public int decodeFlux(byte premier, byte deuxieme) {
		int mdp1 = (int)premier & 0xFF;
		int mdp2 = (int)deuxieme & 0xFF;
		return mdp2*256+mdp1;
	}
	
	//Fonction permettant de recevoir les notifications UDP lorsque le client est connecté au serveur
	public void run() {
		try {
			//Instanciation de la Socket d'écoute et du Paquet récepteur vierge
			byte[] dataReceived = new byte[lengthData];
			DatagramPacket paquetReceived = new DatagramPacket(dataReceived,dataReceived.length);
			try {
			    //C'est ICI qu'on recupere les 3 bytes, decoder et afficher
				while(true) {
						// Reception du paquet (port, adresse), transformation des data reçu en String
						dso.receive(paquetReceived);
						dataReceived = paquetReceived.getData();
						byte type = dataReceived[0];
						int nbflux = decodeFlux(dataReceived[1],dataReceived[2]);
						debugPrintMsgReceived((type + "" + nbflux));
						String typeString = "";
						//Gestion de l'affichager des notifications en fonction de leur type
						switch(type) {
							case 0:
								typeString = "Demande d'amitie";
								break;
							
							case 1:
								typeString = "Demande d'amitie acceptee";
								break;
								
							case 2:
								typeString = "Demande d'amitie refusee";
								break;
								
							case 3:
								typeString = "Nouveau message";
								break;
								
							case 4:
								typeString = "Innondation";
								break;
								
							default:
								typeString = "Inconnue";
								break;
						}
						System.out.println("\n---Notification recue: " + typeString + " - Vous avez " + nbflux + " notifications---"); //Affichage de la notification
					}
			} catch(SocketException e) {
				debugPrint("Fermeture du port d'ecoute UDP"); //DEBUG
			} catch(Exception e) {
				// e.printStackTrace(); //DEBUG
			}
			dso.close();
		} catch(Exception e) {
			debugPrint("Arret du thread d'ecoute UDP"); //DEBUG
			// e.printStackTrace(); //DEBUG
		}	    
	}
}