import java.net.*;
import java.io.*;
import java.util.*;

public class ClientTCP {
    
    private Client client;
    
    //Instanciation pour éviter les nombres magiques permettant les tests	//++
	private final int testLimitLengthMsg = 200;
	
    public ClientTCP(Client client) {
        this.client = client;
    }
    
    //Methode permettant d'afficher les DEBUG de base
	public void debugPrint(String msgDebug) {
		if (this.client.getDebugOption() > 0) {
			System.out.println("DEBUG - TCP - " + msgDebug);
		}
	}

	//Methode permettant d'afficher les DEBUG de réception de messages du Serveur
	public void debugPrintMsgReceived(String msgReceived) {
		if (this.client.getDebugOption() > 1) {
			System.out.println("DEBUG - TCP - CLIENT RECEIVED - " + msgReceived);
		}
	}
	

	//Fonction qui se connecte au réseau social du serveur, et qui récupère le portUDPClient
	public boolean connect(String id, int mdp, Socket socket, OutputStream out, InputStream in) {
	    boolean connected = false;
        try{
        	//Initialisation du buffer qui va generer le tableau de byte
        	ByteArrayOutputStream byteArray = new ByteArrayOutputStream();
        	// Initialisation du mdp encode en int[]
        	int[] mdpsend = encodeMdp(mdp);
        	// Initialisation d'une partie message a envoyer
        	String connePlusId = "CONNE " + id + " ";
        	// Transformation du String en tableau d'octet a envoyer
        	byte[] messageByte = connePlusId.getBytes();
        	byteArray.write(messageByte);
        	byteArray.write(mdpsend[0]);
        	byteArray.write(mdpsend[1]);
        	byteArray.write("+++".getBytes());
            //Envoi du message de connection au serveur
            out.write(byteArray.toByteArray());
            out.flush();
        
            // Tableau de byte qu'on recoit avec le read()
            byte[] msgReceivedBytes = new byte[8];
			in.read(msgReceivedBytes);
			// Message recu en String
			String msgReceived = byteToString(msgReceivedBytes);
            debugPrintMsgReceived(msgReceived); //DEBUG
            
            switch(msgReceived) {
                case "HELLO+++":
                    //Le serveur réponds positivement, puis envoi un second message contenant le port UDP du client.
                	//Initialisation du message a recevoir en tableau de byte[]
                	byte[] portReceivedBytes = new byte[4];
                	// Lecture du port recu en tableau de byte
                	in.read(portReceivedBytes,0,4);
                	// Transformation du tableau de byte en String
                	msgReceived = byteToString(portReceivedBytes);
                    String portUDP = msgReceived;
                    this.client.setPortUDP(Integer.parseInt(portUDP)); //On l'enregistre dans notre objet Client
                    connected = true;
                    break;
                
                case "GOBYE+++":
                    break;
                
    			default:
    			    break;
            }
			return connected;
		}
		catch(Exception e) {
			// e.printStackTrace(); //DEBUG
		}
        return false;
	}
	
	//Fonction qui permet de s'enregistrer sur le serveur
	public boolean register(String id, int mdp, int portUDP, Socket socket, OutputStream out, InputStream in) {
	    boolean registered = false;
        try {
        	//Initialisation du buffer qui va generer le tableau de byte
        	ByteArrayOutputStream byteArray = new ByteArrayOutputStream();
        	// Initialisation du mdp encode en short
        	int[] mdpsend = encodeMdp(mdp);

        	// Initialisation du message a envoyer
        	String regisIdPortUDP = "REGIS " + id + " " + portUDP + " " ;
        	// Transformation du String en tableau d'octet a envoyer
        	byte[] messageByte = regisIdPortUDP.getBytes();
            //Envoi du message de connection au serveur 
        	byteArray.write(messageByte);
        	byteArray.write(mdpsend[0]);
        	byteArray.write(mdpsend[1]);
        	byteArray.write("+++".getBytes());

            out.write(byteArray.toByteArray());
            out.flush();
        
            // Tableau de byte qu'on recoit avec le read() de 8 caractere (GOBYE+++ ou WELCO+++)
            byte[] msgReceivedBytes = new byte[8];
			in.read(msgReceivedBytes);
			// Message recu en String
			String msgReceived = byteToString(msgReceivedBytes);
			debugPrintMsgReceived(msgReceived); //DEBUG
            
            switch(msgReceived) {
                case "WELCO+++":
                    //Le serveur réponds positivement
                    registered = true;
                    this.client.setPortUDP(portUDP); //On entregistre le portUDP dans le Client
                    break;
                
                case "GOBYE+++":
                    break;
                
    			default:
    			    break;
            }
			return registered;
		}catch(NullPointerException e) {
			System.out.println("ERREUR - On reçoit un null"); //ERREUR
		}
		catch(Exception e) {
			// e.printStackTrace(); //DEBUG
		}
        return false;
	}
	
	//Fonction qui permet de consulter ses messages de notifications
	public void consultation(Socket socket, OutputStream out, InputStream in) { 
		try{
	        //----------------------------------------------------- Envoi Message
			// Initialisation du message a envoyer
	    	String messageString = "CONSU+++";
	    	// Transformation du String en tableau d'octet a envoyer
	    	byte[] messageByte = messageString.getBytes();
	        //Envoi du message de consultation au serveur 
	        out.write(messageByte);
	        out.flush();
	        //----------------------------------------------------- Reception Message
            // Tableau de byte qu'on recoit avec le read() de 8 caractere (GOBYE+++ ou WELCO+++)
            byte[] msgReceivedBytes = new byte[50];
			//Position de la lecture 
			int offset = 0;
			String troisplus = "+++";
			//Tableau de byte[] contenant les 3 derniers bytes lus
			byte[] troisdernier = new byte[3];
			//String qui contient les 3 deniers bytes lus (a comparer avec troisplus)
			String acomparer;
            
			try{
				//On recupere les bytes qu'on stocke dans msgReceived jusqu'a recevoir +++
				while(true) {
					//On lis byte par byte
					in.read(msgReceivedBytes, offset, 1);
					//Si on a au moins 3 bytes lus
					if(offset>3) {
						//On compare les 3 derniers bytes avec +++
						troisdernier[0] = msgReceivedBytes[offset-2];
						troisdernier[1] = msgReceivedBytes[offset-1];
						troisdernier[2] = msgReceivedBytes[offset];
						acomparer = new String(troisdernier);
						if(acomparer.equals(troisplus)) {
							offset++;
							break;
						}
					}
					offset++;
				}
			}catch(Exception e){
				// e.printStackTrace(); //DEBUG
			}

			String idRecu;
			// Message recu en String
			String msgReceived = byteToString(msgReceivedBytes,0,offset);
			debugPrintMsgReceived(msgReceived); //DEBUG
			String[] msgReceivedSplit = msgReceived.split(" ");
			//Taille du message recu en octets
			int tailleMsgReceived = offset;
			
			//Gestion des messages de consultation en fonction du premier mot
			switch(msgReceivedSplit[0]){
				case "FRIEN":
					//Affichage de l'id qui a accepte la demande d'ami
					idRecu = msgReceivedSplit[1].substring(0,8);
					System.out.println(idRecu + " a accepte votre demande d'amitie.");
					break;
	//--------------------------------------------------------------------------------------------------------------------------------------
				case "NOFRI":
					//Affichage de l'id qui a refuse la demande d'ami
					idRecu = msgReceivedSplit[1].substring(0,8);
					System.out.println(idRecu + " a refuse votre demande d'amitie.");
					break;
	//--------------------------------------------------------------------------------------------------------------------------------------
				case "EIRF>":
					idRecu = msgReceivedSplit[1].substring(0,8);
					System.out.print(idRecu + " vous a fait une demande d'amitie.\nTapez '1' pour l'accepter, '2' pour refuser: ");
					Scanner sc = new Scanner(System.in);
					System.out.println(); //SEPARATEUR
					int reponse = 2;
					try{
						reponse = sc.nextInt();
					} catch (Exception e) {
						System.out.println("Mauvaise valeur, une reponse negative sera transmise");
					}
					if(reponse==1) {
						messageString = "OKIRF+++";
					} else {
						messageString = "NOKRF+++";
					}
					messageByte = messageString.getBytes();
		    		// Envoi du message au serveur 
		        	out.write(messageByte);
		        	out.flush();
		        	try{
						//On lis 8 bytes (cense recevoir "ACKRF+++")
						in.read(msgReceivedBytes, 0, 8);
						msgReceived = byteToString(msgReceivedBytes, 0, 8); //TODO verifier qu'on a les bon indices ?
						debugPrintMsgReceived(msgReceived); //DEBUG
						if(msgReceived.equals("ACKRF+++")) {
							//Acquitement du message
						} else {
							//Non aquitement du message
						}
		        	}catch(Exception e){
		        		debugPrint("DEBUG - Mauvais message recu du Serveur"); //DEBUG
		        	}
					break;
	//--------------------------------------------------------------------------------------------------------------------------------------
				case "SSEM>":
					//On vérifie la taille du message reçu qui doit être de 22 octets
					if(tailleMsgReceived != 22) {
						System.out.println("ERREUR - Le message que le client envoi n'est pas de la forme 'SSEM>id num-mess+++'"); //ERREUR
						break;
					} else {
						//On récupère l'id de l'ami, le nb de message a recup, et on cree le msg final qui sera affiche
						String idFriendSender = msgReceivedSplit[1];
						int nbMessCut = Integer.parseInt(msgReceivedSplit[2].substring(0, msgReceivedSplit[2].length()-3));
						String msgToRead = "";

						//On récupère chaque message pour l'ajouter au message final
						for (int i = 0; i < nbMessCut; i++) {
							//Reinitialisation de l'offset
							offset = 0;
		
							//On recupere les bytes qu'on stocke dans msgReceived jusqu'a recevoir +++
							while(true) {
								//On lis byte par byte
								in.read(msgReceivedBytes, offset, 1);
								//Si on a au moins 3 bytes lus
								if(offset>3) {
									//On compare les 3 derniers bytes avec +++
									troisdernier[0] = msgReceivedBytes[offset-2];
									troisdernier[1] = msgReceivedBytes[offset-1];
									troisdernier[2] = msgReceivedBytes[offset];
									acomparer = new String(troisdernier);
									if(acomparer.equals(troisplus)) {
										offset++;
										break;
									}
								}
								offset++;
							}
							//Taille du message recu en octets
							tailleMsgReceived = offset;
		
							//Conversion du tableau de byte en String (seulement ce qu'on a lus)
							msgReceived = byteToString(msgReceivedBytes,0 , tailleMsgReceived);
							debugPrintMsgReceived(msgReceived); //DEBUG
							msgToRead = msgToRead + msgReceived.substring(11, msgReceived.length()-3);
						}
						System.out.println("Message reçu de la part de " + idFriendSender + ":\n" + msgToRead);
					}
					break;
	//--------------------------------------------------------------------------------------------------------------------------------------
				case "OOLF>":
					String msgFlood = msgReceived.substring(15, msgReceived.length()-3);
					String idFlood = msgReceived.substring(6, 14);
					System.out.println("Message d'inondation reçu de la part de " + idFlood + ":\n" + msgFlood);
					break;
	//--------------------------------------------------------------------------------------------------------------------------------------
				case "NOCON+++":
					System.out.println("Pas de consultation dans la liste de flux.");
					break;
					
				default:
					break;
			}
		}catch(Exception e) {
			// e.printStackTrace(); //DEBUG
		}
	}	
	
	//Fonction qui permet d'envoyer un message
	public boolean sendMessage(String msgForFriend, String friendIdToSendMsg, Socket socket, OutputStream out, InputStream in) {
		try {
			//On détermine le nombre de msg à envoyer et on code ce nombre sur 4 octets en complétant avec des 0
			String nb0ToAdd = "";
			int nbMsg = msgForFriend.length() / this.testLimitLengthMsg;
			if ((msgForFriend.length() % this.testLimitLengthMsg) > 0) {
				nbMsg++;
			}
			if (nbMsg < 10) {
				nb0ToAdd = "000";
			} else if (nbMsg < 100) {
				nb0ToAdd = "00";
			} else if (nbMsg < 1000) {
				nb0ToAdd = "0";
			}
			
			// Initialisation du message a envoyer
        	String messageString = "MESS? " + friendIdToSendMsg + " " + nb0ToAdd + nbMsg + "+++";
        	// Transformation du String en tableau d'octet a envoyer
        	byte[] messageByte = messageString.getBytes();
            //Envoi du message de connection au serveur 
            out.write(messageByte);
            out.flush();
            
			//Envoi de tous les messages découpés, sauf le dernier
			nb0ToAdd = "000";
			for (int numMsgCut = 0; numMsgCut < nbMsg-1; numMsgCut++) {
				//On code le nombre de message sur 4 octets en ajoutant des 0
				if (numMsgCut == 10) {
					nb0ToAdd = "00";
				} else if (numMsgCut == 100) {
					nb0ToAdd = "0";
				} else if (numMsgCut == 1000) {
					nb0ToAdd = "";
				}
				
				// Initialisation du message a envoyer
				messageString = "MENUM " + nb0ToAdd + numMsgCut + " " + msgForFriend.substring(0, this.testLimitLengthMsg) + "+++";
				// Transformation du String en tableau d'octet a envoyer
				messageByte = messageString.getBytes();
				//Envoi du message de connection au serveur 
				out.write(messageByte);
				out.flush();
				msgForFriend = msgForFriend.substring(this.testLimitLengthMsg, msgForFriend.length());
			}
			
			//Envoi du dernier message découpé
			messageString = "MENUM " + nb0ToAdd + (nbMsg -1) + " " + msgForFriend + "+++";
			// Transformation du String en tableau d'octet a envoyer
			messageByte = messageString.getBytes();
			//Envoi du message de connection au serveur 
			out.write(messageByte);
			out.flush();
			
			// Tableau de byte qu'on recoit avec le read() de 8 caractere (MESS>+++ ou MESS<+++)
            byte[] msgReceivedBytes = new byte[8];
			in.read(msgReceivedBytes);
			// Message recu en String
			String msgReceived = byteToString(msgReceivedBytes);
            debugPrintMsgReceived(msgReceived); //DEBUG
            
            if (msgReceived.equals("MESS>+++")) {
		 		return true;
            } else if (msgReceived.equals("MESS<+++")) {
				//false est return à la fin
            } else {
            	System.out.println("ERREUR - Le message recu n'est pas 'MESS>+++' ou 'MESS<+++'!"); //ERREUR
            }
		} catch(Exception e) {
			System.out.println("ERREUR - Le message n'a pas pu être envoyé!"); //ERREUR
			// e.printStackTrace(); //DEBUG
		}
	    return false;
	}
	
	//Fonction qui permet d'ajouter un ami
	public boolean addFriend(String id, Socket socket, OutputStream out, InputStream in) {
		try {
			// Initialisation du message a envoyer
        	String messageString = "FRIE? " + id + "+++";
        	// Transformation du String en tableau d'octet a envoyer
        	byte[] messageByte = messageString.getBytes();
            //Envoi du message de connection au serveur 
            out.write(messageByte);
            out.flush();
            
            // Tableau de byte qu'on recoit avec le read() de 8 caractere (GOBYE+++ ou WELCO+++)
            byte[] msgReceivedBytes = new byte[8];
			in.read(msgReceivedBytes);
			// Message recu en String
			String msgReceived = byteToString(msgReceivedBytes);
			debugPrintMsgReceived(msgReceived); //DEBUG
            
            if(msgReceived.equals("FRIE>+++")) {
		 		System.out.println("Demande d'amitie effectuee.");
		 		return true;
            }
            else if(msgReceived.equals("FRIE<+++")) {
            	System.out.println("ERREUR - Echec de la demande d'amitiee avec: " + id + "!"); //ERREUR
            	return false;
            }
            else {
            	System.out.println("ERREUR - Le message recu est de la forme: " + msgReceived ); //ERREUR
            	return false;
            }
		} catch(Exception e) {
			// e.printStackTrace(); //DEBUG
		}
		return false;
	}
	
	//Fonction qui permet de recuperer la liste de tous les users
	public ArrayList<String> getListUsers(Socket socket, OutputStream out, InputStream in) {
	    try{
	    	//Initialisation de la liste d'users à renvoyer
	    	ArrayList<String> listUsers = new ArrayList<String>();
	    	
			//Initialisation du message a envoyer
        	String messageString = "LIST?+++";
        	
        	//Transformation du String en tableau d'octet a envoyer
        	byte[] messageByte = messageString.getBytes();
        	
            //Envoi du message de connection au serveur 
            out.write(messageByte);
            out.flush();
            
            //Tableau de byte qu'on recoit avec le read() de taille 9char et 3char pour num-item (RLIST␣num-item+++)
            byte[] msgReceivedBytes = new byte[12];
			in.read(msgReceivedBytes);
			
			//Message reçu en String
			String msgReceived = byteToString(msgReceivedBytes);
            debugPrintMsgReceived(msgReceived); //DEBUG
            
            //On récupère le nombre d'users
            int nbUsers = Integer.parseInt(msgReceived.split(" ")[1].substring(0, msgReceived.split(" ")[1].length()-3));

            //On ajoute chacun des users que le serveur envoit à la liste finale
            for (int i=0; i<nbUsers; i++) {
				//Tableau de byte qu'on recoit avec le read() de taille 17char (LINUM␣id+++)
				msgReceivedBytes = new byte[17];
				in.read(msgReceivedBytes);
				
				//Message reçu en String
				msgReceived = byteToString(msgReceivedBytes);
				debugPrintMsgReceived(msgReceived); //DEBUG
				
				//On ajoute l'user à la liste
				listUsers.add(msgReceived.split(" ")[1].substring(0, msgReceived.split(" ")[1].length()-3));
            }
            
			return listUsers;
		} catch(Exception e){
			// e.printStackTrace(); //DEBUG
		}
		System.out.println("ERREUR - La liste des users est erronée"); //ERREUR
		return null;
	}
	
	//Fonction permettant d'envoyer un message recursivement aux amis et leurs amis etc
	public boolean flood(String message,Socket socket, OutputStream out, InputStream in) {
		try{
        	// Initialisation d'une partie message a envoyer
        	String msgTosend = "FLOO?" + " "+ message + "+++";
        	// Transformation du String en tableau d'octet a envoyer
        	byte[] messageByte = msgTosend.getBytes();
            //Envoi du message de flood au serveur
            out.write(messageByte);
            out.flush();
            
            //Tableau de byte qu'on recoit avec le read() de taille 9char et 3char pour num-item (RLIST␣num-item+++)
            byte[] msgReceivedBytes = new byte[8];
			in.read(msgReceivedBytes);
			
			//Message reçu en String
			String msgReceived = byteToString(msgReceivedBytes);
            debugPrintMsgReceived(msgReceived); //DEBUG
            
            if(msgReceived.equals("FLOO>+++")) {
            	return true;
            }else {
            	return false;
            }
		} catch(Exception e) {}
		return false;
	}
	
	//Fonction qui permet de se deconnecter du reseau social
	public boolean disconnect(Socket socket, OutputStream out, InputStream in) {
		boolean exit = false;
		try{
			// Initialisation du message a envoyer
        	String messageString = "IQUIT+++";
        	// Transformation du String en tableau d'octet a envoyer
        	byte[] messageByte = messageString.getBytes();
            //Envoi du message de connection au serveur 
            out.write(messageByte);
            out.flush();
            
            // Tableau de byte qu'on recoit avec le read() de 8 caractere (GOBYE+++)
            byte[] msgReceivedBytes = new byte[8];
			in.read(msgReceivedBytes);
			// Message recu en String
			String msgReceived = byteToString(msgReceivedBytes);
			debugPrintMsgReceived(msgReceived); //DEBUG
            
            if(msgReceived.equals("GOBYE+++")) {
            	this.client.disconnectServer();
		 		debugPrint("Deconnection du Client"); //DEBUG
		 		exit = true;
            }
            else {
            	System.out.println("ERREUR - Le message recu n'est pas GOBYE+++"); //ERREUR
            }
            return exit;
		} catch (SocketException e) {
			//Si la socket se ferme de force
			return false;
		} catch(Exception e) {
			// e.printStackTrace(); //DEBUG
		}
		return false;
	}
	
	//------------ FONCTIONS D'ENCODAGEs ------------ 
    //Fonction qui encode le mdp en little endian et le mets dans un short (2 octets)
    public int[] encodeMdp(int mdp) {
    	int[] mdpLE = new int[2];
    	mdpLE[0] = mdp%256;
    	mdpLE[1] = mdp/256;
		return mdpLE;
    }
	
	//Cree un String a partir d'un tableau de Bytes
	public String byteToString(byte[] messageByte) {
		return new String(messageByte);
	}
	
	//Cree un String en prenant les valeurs du tableau de byte de debut a offset(fin)
	public String byteToString(byte[] messageByte,int debut, int offset) {
		return new String(messageByte,debut,offset);
	}
	//------------------------------------------------
}