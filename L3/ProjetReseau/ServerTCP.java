import java.net.*;
import java.io.*;
import java.util.*;


public class ServerTCP implements Runnable {

	private Server server;
	private Socket socket;

	//Constructeur du thread ServerTCP
	public ServerTCP(Server server, Socket socket) {
		this.server = server;
		this.socket = socket;
	}

	//Methode permettant d'afficher les DEBUG de base
	public void debugPrint(String msgDebug) {
		if (this.server.getDebugOption() > 0) {
			System.out.println("DEBUG - TCP - " + msgDebug);
		}
	}
	
	//Methode permettant d'afficher les DEBUG de réception de messages du Client
	public void debugPrintMsgReceived(String msgReceived) {
		if (this.server.getDebugOption() > 1) {
			System.out.println("DEBUG - TCP - SERVER RECEIVED - " + msgReceived);
		}
	}
	
	
	//Methode d'ajout d'un client s'il n'existe pas deja, renvoi un boolean true si succes, false sinon
	public boolean addUser(Utilisateur user) {
		if (this.server.getListClients().size() == this.server.getLimitHashtable()) {
			//Si le nombre de client atteint la limite du serveur on refuse l'ajout
			debugPrint("Il y a trop de Client dans la liste"); //DEBUG
			return false;
		} else if (!this.server.getListClients().containsKey(user.getId())) {
			if(this.server.getListPortUDP().containsKey(user.getPortUDP())) {
				debugPrint("Port indiponible"); //DEBUG
				return false;
			} else {
				//L'ajout est accepte si l'id n'est pas deja utilise
				this.server.getListClients().put(user.getId(), user);
				//L'ajout est accepte si l'id n'est pas deja utilise
				this.server.getListPortUDP().put(user.getPortUDP(), "");
				debugPrint("Client ajoute avec succes!"); //DEBUG
				debugPrint("Liste des clients: " + this.server.getListClients()); //DEBUG
				return true;
			}
		} else {
			//Sinon il est refuse
			System.out.println("ERREUR - Le client " + user.getId() + " existe deja! Liste des clients: " + this.server.getListClients()); //ERREUR
			return false;
		}
	}

	//Methode permettant de dire si le couple id/mdp correspond dans les utilisateurs inscrits
	public boolean userConnect(String id, int mdp) {
		if (this.server.getListClients().containsKey(id) && this.server.getListClients().get(id).getMdp()==mdp) {
			return true;
		}
		return false;
	}

	//Methode permettant de tester si la demande d'ami est faisable (auto-ajout et doublon impossible)
	public boolean demandeAmiList(Utilisateur user, String id) {
		if(!(server.getListClients().containsKey(id))) {
			debugPrint("Cet utilisateur n'existe pas et ne peux donc pas être ajouté"); //DEBUG
			return false;
		} else{
			if(user.getId().equals(id)) {
				debugPrint(user.getId() + " ne peut pas s'ajouter lui meme en ami"); //DEBUG
				return false;
			} else if (!user.isAmi(id)) {
				return true;
			} else {
				debugPrint(user.getId() + " est deja ami avec " + id); //DEBUG
				return false;
			}
		}
	}
	
	//Methode permettant de remplir un HashSet contenant au moins un user avec tous ses amis récursifs (amis de leur amis etc..)
	public void getRecursiveFriends(HashSet<String> listRecursiveFriends) {
		try {
			for (String friend: listRecursiveFriends) {
				if(!listRecursiveFriends.addAll(this.server.getListClients().get(friend).getListAmis())) {
				} else {
					getRecursiveFriends(listRecursiveFriends);
				}
			}
		} catch (Exception e) {}
	}

	//Lancement du thread TCP
	public void run() {
		try {
			//Creation du boolean permettant de garder le client connecte au serveur et donc sur le meme thread
			boolean stayConnected = true;

			//Instanciation du InputStream et du OutputStream
			InputStream inputStream = socket.getInputStream();
			OutputStream outputStream= socket.getOutputStream();

			try {
				//Message qu'on recoit en tableau de byte[]
				byte[] msgReceivedByte = new byte[512];
				//Position de la lecture 
				int offset = 0;
				String troisplus = "+++";
				//Tableau de byte[] contenant les 3 derniers bytes lus
				byte[] troisdernier = new byte[3];
				//String qui contient les 3 deniers bytes lus (a comparer avec troisplus)
				String acomparer;
				int tailleMsgReceived = 0;

				//Instanciation du message a envoyer
				byte[] msgToSendByte = new byte[8];
				String msgToSend;
				//Instanciation de l'id, du mdp et du user
				String id;
				int mdp;
				Utilisateur user = null;
				//Instanciation de variable temporaire pour le decodage du mdp
				byte[] mdpByte = new byte[2];
				// Utilisateur auquel on va envoyer la notif UDP
				Utilisateur destinataireUserUDP = null;
				//Byte du type Y UDP
				byte typeUDP;

				//Tableau de byte a envoyer
				byte[] msgConsuTosend = new byte[214];
				//Objet qu'on va ajouter comme flux (message + id de l'expediteur)
				Consu consuAdd;
				//Objet qu'on va utiliser pour envoyer la reponse a CONSU
				Consu consuTreat;

				while(stayConnected) {
					//Reinitialisation de l'offset
					offset = 0;

					//On recupere les bytes qu'on stocke dans msgReceived jusqu'a recevoir +++
					while(true) {
						//On lis byte par byte
						inputStream.read(msgReceivedByte, offset, 1);
						//Si on a au moins 3 bytes lus
						if(offset>3) {
							//On compare les 3 derniers bytes avec +++
							troisdernier[0] = msgReceivedByte[offset-2];
							troisdernier[1] = msgReceivedByte[offset-1];
							troisdernier[2] = msgReceivedByte[offset];
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
					String msgReceived = byteToString(msgReceivedByte,0,tailleMsgReceived);
					debugPrintMsgReceived(msgReceived); //DEBUG
					//Separation du message avec les espaces pour avoir le type de message (5 premiers octets/caracteres)
					String[] msgReceivedSplit = msgReceived.split(" ");

					//On traite les messages du client en fonction du premier mot
					switch(msgReceivedSplit[0]) {
						//Connection au reseau social
						case "CONNE":
							//On verifie qu'on a bien le bon nombre d'octets
							if(tailleMsgReceived != 20) {
								msgToSend = "GOBYE+++";
								stayConnected = false;
								msgToSendByte = msgToSend.getBytes();
								outputStream.write(msgToSendByte);
								outputStream.flush();
								break;
							}
							id = msgReceivedSplit[1];
							//Tableau des 2 bytes correpondant au mdp
							mdpByte[0] = msgReceivedByte[tailleMsgReceived-5];
							mdpByte[1] = msgReceivedByte[tailleMsgReceived-4];
							mdp = decodeMdp(mdpByte[0], mdpByte[1]);
							// debugPrint("Mot de passe decode : " + mdp); //DEBUG
							if (userConnect(id, mdp)) {
								//On repond une premiere fois HELLO avant de repondre une seconde fois le port UDP du Client
								msgToSend = "HELLO+++";
								msgToSendByte = msgToSend.getBytes();
								outputStream.write(msgToSendByte);
								outputStream.flush();
	
								msgToSend = String.valueOf(this.server.getListClients().get(id).getPortUDP());
								//On repond une deuxieme fois en envoyant le portUDP
								byte[] portToSendByte = msgToSend.getBytes();
								outputStream.write(portToSendByte);
								outputStream.flush();
	
								//On actualise l'utilisateur courant pour les actions suivantes
								user = this.server.getUser(id);
							} else {
								//On repond seulement GOBYE
								msgToSend = "GOBYE+++";
								msgToSendByte = msgToSend.getBytes();
								outputStream.write(msgToSendByte);
								outputStream.flush();
							}
							break;
		//--------------------------------------------------------------------------------------------------------------------------------------
						//Inscription au reseau social
						case "REGIS":
							//Si on a pas le bon nombre d'octets on envoi GOBYE
							if(tailleMsgReceived != 25) { 
								msgToSend = "GOBYE+++";
								stayConnected = false;
								msgToSendByte = msgToSend.getBytes();
								outputStream.write(msgToSendByte);
								outputStream.flush();
								break;
							}
							//On cree un nouvel utilisateur
							id = msgReceivedSplit[1];
							//Tableau des 2 bytes correpondant au mdp
							mdpByte[0] = msgReceivedByte[tailleMsgReceived-5];
							mdpByte[1] = msgReceivedByte[tailleMsgReceived-4];
							// String mdpString = String.valueOf(mdp1)+String.valueOf(mdp2); //DEBUG
							// debugPrint("Mot de passe recupere String : " + mdpString); //DEBUG
							mdp = decodeMdp(mdpByte[0], mdpByte[1]);
							// debugPrint("Mot de passe decode int : " + mdp); //DEBUG
							int portUDP = Integer.parseInt(msgReceivedSplit[2]);
							String adresse = socket.getInetAddress().getHostAddress();
							user = new Utilisateur(id,mdp,portUDP,adresse);
	
							//On tente d'ajouter le client au serveur
							if(addUser(user)) {
								msgToSend = "WELCO+++";
							} else {
								msgToSend = "GOBYE+++";
								stayConnected = false;
							}
	
							//On repond en fonction de si l'ajout e ete fait ou non
							msgToSendByte = msgToSend.getBytes();
							outputStream.write(msgToSendByte);
							outputStream.flush();
							break;
	//--------------------------------------------------------------------------------------------------------------------------------------
						//Deconnexion du reseau social
						case "IQUIT+++":
							msgToSend = "GOBYE+++";
							msgToSendByte = msgToSend.getBytes();
							outputStream.write(msgToSendByte);
							outputStream.flush();
							stayConnected = false;
							break;
	//--------------------------------------------------------------------------------------------------------------------------------------
						//Demande d'ami
						case "FRIE?":
							if(tailleMsgReceived != 17) {
								msgToSend = "FRIE<+++"; //TEST
								msgToSendByte = msgToSend.getBytes();
								outputStream.write(msgToSendByte);
								outputStream.flush();
								break;
							}
							//Id de l'utilisateur qu'on veut demander en ami
							String idAmitie = msgReceivedSplit[1].substring(0,msgReceivedSplit[1].length()-3);
							//Utilisateur courant (le dernier qui s'est connecte ou enrengistre)
							//On initialise le message a envoyer en fonction du succes de l'ajout d'amis
							if(demandeAmiList(user,idAmitie)) {
								destinataireUserUDP = server.getUser(idAmitie);
								typeUDP = 0;
								//Envoi d'une notification a la personne qui a une demande d'ami
								this.server.getServerUDP().sendUDP(typeUDP, destinataireUserUDP);
								//Genere la consultation (id de l'expediteur + message en byte[])
								consuAdd = new Consu(user.getId(), msgReceivedByte, 0, tailleMsgReceived);
								//Ajoute au flux du message recu en byte
								if(destinataireUserUDP.addConsu(consuAdd)) {
									msgToSend = "FRIE>+++";
									debugPrint(user.getId() + " demmande " + idAmitie + " en ami.\nListe des consu de " + destinataireUserUDP.getId() + ": " + destinataireUserUDP.getListConsu()); //DEBUG
								} else {
									msgToSend = "FRIE<+++";
								}
							} else {
								msgToSend = "FRIE<+++";
							}
							//On envoi le message sous forme de tableau de byte[]
							msgToSendByte = msgToSend.getBytes();
							outputStream.write(msgToSendByte);
							outputStream.flush();
							break;
	//--------------------------------------------------------------------------------------------------------------------------------------
						//Demande des utilisateurs
						case "LIST?+++":
							//Recuparation des id d'users, (keys de la Hashtable d'user)
							Set idUsers = this.server.getListClients().keySet();
							
							//On traite le nombre recupere pour ajouter des "0" au debut si besoin afin de suivre le protocole
							String nbUsers = String.valueOf(idUsers.size());
							if (idUsers.size() < 10) {
								nbUsers = "00" + nbUsers;
							} else if (idUsers.size() < 100) {
								nbUsers = "0" + nbUsers;
							}
							debugPrint("Listes des users: " + nbUsers + " " + idUsers); //DEBUG
							
							//On envoi le message contenant le nombre d'utilisateurs
							msgToSend = "RLIST " + nbUsers + "+++";
							msgToSendByte = msgToSend.getBytes();
							outputStream.write(msgToSendByte);
							outputStream.flush();
							
							//On envoi tous les id d'users un par un
							for (Object idUser: idUsers) {
								msgToSend = "LINUM " + idUser + "+++";
								msgToSendByte = msgToSend.getBytes();
								outputStream.write(msgToSendByte);
								outputStream.flush();
							}
							break;
	//--------------------------------------------------------------------------------------------------------------------------------------
						//Reponse positive d'une demande d'ami	
						case "OKIRF+++":
							msgToSend = "ACKRF+++";
							//On envoi le message sous forme de tableau de byte[]
							msgToSendByte = msgToSend.getBytes();
							outputStream.write(msgToSendByte);
							outputStream.flush();
							//Ajout d'amis dans les 2 listes d'amis des 2 utilisateurs
							user.addAmi(destinataireUserUDP.getId());
							destinataireUserUDP.addAmi(user.getId());
							//Envoi de la notif de la reponse d'amitie
							typeUDP = 1;
							this.server.getServerUDP().sendUDP(typeUDP, destinataireUserUDP);
							//Genere la consultation (id de l'expediteur + message en byte[])
							consuAdd = new Consu(user.getId(), msgReceivedByte, 0, tailleMsgReceived);
							//Ajoute au flux du message recu en byte
							destinataireUserUDP.addConsu(consuAdd);
							break;
	//--------------------------------------------------------------------------------------------------------------------------------------
						//Reponse negative d'une demande d'ami	
						case "NOKRF+++":
							msgToSend = "ACKRF+++";
							//On envoi le message sous forme de tableau de byte[]
							msgToSendByte = msgToSend.getBytes();
							outputStream.write(msgToSendByte);
							outputStream.flush();
							//Envoi de la notif de la reponse d'amitie
							typeUDP = 2;
							this.server.getServerUDP().sendUDP(typeUDP, destinataireUserUDP);
							//Genere la consultation (id de l'expediteur + message en byte[])
							consuAdd = new Consu(user.getId(), msgReceivedByte, 0, tailleMsgReceived);
							//Ajoute au flux du message recu en byte
							destinataireUserUDP.addConsu(consuAdd);
							break;
	//--------------------------------------------------------------------------------------------------------------------------------------
						//Demande de Flood
						case "FLOO?" :
							msgToSend = "FLOO>+++";
							//On envoi le message sous forme de tableau de byte[]
							msgToSendByte = msgToSend.getBytes();
							outputStream.write(msgToSendByte);
							outputStream.flush();
							
							//On ajout la liste des amis de l'user à la liste de tous les utilisateurs qui seront flood, ainsi que l'utilisateur lui même pour éviter des boucles
							HashSet<String> listFlood = user.getListAmis();
							listFlood.add(user.getId());
							
							//On boucle sur cette liste et on ajoute chaque ami ayant une liaison mais ne s'y trouvant pas
							getRecursiveFriends(listFlood);
							debugPrint("Liste des utilisateurs a flood: " + listFlood.toString()); //DEBUG
							
							//On envoi une notification a chacun des amis, sauf à l'utilisateur lui même
							for(String userToFlood : listFlood){
								if (!userToFlood.equals(user.getId())) {
									destinataireUserUDP = server.getUser(userToFlood);
									typeUDP = 4;
									//Envoi d'une notification a la personne qui a une demande d'ami
									this.server.getServerUDP().sendUDP(typeUDP, destinataireUserUDP);
									//Genere la consultation (id de l'expediteur + message en byte[])
									consuAdd = new Consu(user.getId(), msgReceivedByte, 0, tailleMsgReceived);
									destinataireUserUDP.addConsu(consuAdd);
								}
							}
							break;
	//--------------------------------------------------------------------------------------------------------------------------------------
						//Envoi de message
						case "MESS?":
							//On vérifie la taille du message reçu qui doit être de 22 octets
							if(tailleMsgReceived != 22) {
								//Si ce n'est pas le cas on dit que le message ne s'est pas envoyé
								System.out.println("ERREUR - Le message que le client envoi n'est pas de la forme 'MESS?id num-mess+++'"); //ERREUR
								msgToSend = "MESS<+++";
								//On envoi le message sous forme de tableau de byte[]
								msgToSendByte = msgToSend.getBytes();
								outputStream.write(msgToSendByte);
								outputStream.flush();
								break;
							} else {
								//Si la taille est bonne, on vérifie que les utilisateurs sont amis, sinon le message ne s'envoit pas
								String idFriendToSend = msgReceivedSplit[1];
								if (user.isAmi(idFriendToSend)) {
									//On crée la consu qui sera la concaténation de tous les message
									String msgToSave = msgReceived;
									
									//On récupère le nombre de messages à récuperer
									int nbMessCut = Integer.parseInt(msgReceivedSplit[2].substring(0, msgReceivedSplit[2].length()-3));
									
									for (int i = 0; i < nbMessCut; i++) {
										//Reinitialisation de l'offset
										offset = 0;
					
										//On recupere les bytes qu'on stocke dans msgReceived jusqu'a recevoir +++
										while(true) {
											//On lis byte par byte
											inputStream.read(msgReceivedByte, offset, 1);
											//Si on a au moins 3 bytes lus
											if(offset>3) {
												//On compare les 3 derniers bytes avec +++
												troisdernier[0] = msgReceivedByte[offset-2];
												troisdernier[1] = msgReceivedByte[offset-1];
												troisdernier[2] = msgReceivedByte[offset];
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
										msgReceived = byteToString(msgReceivedByte,0,tailleMsgReceived);
										debugPrintMsgReceived(msgReceived); //DEBUG
										msgToSave = msgToSave + " " +msgReceived;
									}
									// debugPrint("Message final : " + msgToSave); //DEBUG
									byte[] msgToSaveByte = msgToSave.getBytes();
									
									//Envoi de la notif d'un nouveau message
									typeUDP = 3;
									destinataireUserUDP = server.getUser(idFriendToSend);
									this.server.getServerUDP().sendUDP(typeUDP, destinataireUserUDP);
	
									//Si l'ajout du message au flux de l'utilisateur s'est fait correctement
									//Genere la consultation (id de l'expediteur + message en byte[])
									consuAdd = new Consu(user.getId(), msgToSaveByte, 0, msgToSaveByte.length);
									if (destinataireUserUDP.addConsu(consuAdd)) {
										msgToSend = "MESS>+++";
										debugPrint("Le message s'est envoye correctement et a ete ajoute au flux"); //DEBUG
									} else {
										msgToSend = "MESS<+++";
										//On envoi le message sous forme de tableau de byte[]
										System.out.println("ERREUR - L'ajout de la consultation au flux a echouee"); //ERREUR
									}
									//On envoi le message sous forme de tableau de byte[]
									msgToSendByte = msgToSend.getBytes();
									outputStream.write(msgToSendByte);
									outputStream.flush();
								} else {
									msgToSend = "MESS<+++";
									//On envoi le message sous forme de tableau de byte[]
									System.out.println("ERREUR - " + user.getId() + " et " + idFriendToSend + " ne sont pas amis!"); //ERREUR
									msgToSendByte = msgToSend.getBytes();
									outputStream.write(msgToSendByte);
									outputStream.flush();
								}
							}
							break;
	//--------------------------------------------------------------------------------------------------------------------------------------
						//Consultation des notifications
						case "CONSU+++":
							String msgToConsuString;
							try{
								//On recupere la consultation (id + message)
								consuTreat = user.getConsu();
								//Si il n'y a pas de Consu on renvoit NOCON
								if (consuTreat == null) {
									msgToSend = "NOCON+++";
									//On envoi le message sous forme de tableau de byte[]
									msgToSendByte = msgToSend.getBytes();
									outputStream.write(msgToSendByte);
									outputStream.flush();
									break;
								}
								//On decremente le flux
								//On recupere message en byte[] et id separe
								byte[] msgCaseConsu = consuTreat.getMessage();
								String idConsu = consuTreat.getId();
								//On recupere le message en String
								String caseConsuString = byteToString(msgCaseConsu, 0, msgCaseConsu.length);
								debugPrint(user.getId() + " recupere la consultation: " + caseConsuString); //DEBUG
								//Split pour switcher en fonction du type de message de consultation
								String[] typeConsuSplit = caseConsuString.split(" ");
								//Longueur du message a envoyer
								int longueurMsgTosend;
								
								//On traite les consultations en fonction du premier mot
								switch(typeConsuSplit[0]) {
									//Consultation d'un message
									case "MESS?":
										//On recupere le nombre de message pour boucler avec
										int nbMess = Integer.parseInt(typeConsuSplit[2].substring(0,typeConsuSplit[2].length()-3));
										//Message String a envoyer
										msgToConsuString = "SSEM> " + idConsu + " " + typeConsuSplit[2].substring(0,typeConsuSplit[2].length()-3) + "+++";
										//Message byte[] a envoyer
										msgConsuTosend = msgToConsuString.getBytes();
										//Longueur du message a envoyer
										longueurMsgTosend = msgToConsuString.length();
										//Envoi le message de taille longueurMsgTosend
										outputStream.write(msgConsuTosend,0,longueurMsgTosend);
										outputStream.flush();
										
										String[] messageSplitMunem = caseConsuString.split("MENUM");
										for (int i = 1; i<nbMess; i++){
											//Message String a convertir
											msgToConsuString = "MUNEM" + messageSplitMunem[i].substring(0,messageSplitMunem[i].length()-1);
											//Message byte[] a envoyer
											msgConsuTosend = msgToConsuString.getBytes();
											//Longueur du message a envoyer
											longueurMsgTosend = msgToConsuString.length();
											//Envoi le message de taille longueurMsgTosend
											outputStream.write(msgConsuTosend,0,longueurMsgTosend);
											outputStream.flush();
										}
										msgToConsuString = "MUNEM" + messageSplitMunem[nbMess]; //TODO a garder au cas ou .substring(0,messageSplitMunem[i].length()-1)
										//Message byte[] a envoyer
										msgConsuTosend = msgToConsuString.getBytes();
										//Longueur du message a envoyer
										longueurMsgTosend = msgToConsuString.length();
										//Envoi le message de taille longueurMsgTosend
										outputStream.write(msgConsuTosend,0,longueurMsgTosend);
										outputStream.flush();
										break;
				//----------------------------------------------------------------------------------------						
									//Consultation d'une demande d'ami
									case "FRIE?":
										//Message String a envoyer
										msgToConsuString = "EIRF> " + idConsu + "+++";
										//Message byte[] a envoyer
										msgConsuTosend = msgToConsuString.getBytes();
										//Longueur du message a envoyer
										longueurMsgTosend = msgToConsuString.length();
										//Envoi le message de taille longueurMsgTosend
										outputStream.write(msgConsuTosend,0,longueurMsgTosend);
										outputStream.flush();
										//
										destinataireUserUDP = server.getUser(idConsu);
										break;
				//----------------------------------------------------------------------------------------						
									//Consultation d'une acceptation d'amitie
									case "OKIRF+++":
										//Message String a envoyer
										msgToConsuString = "FRIEN " + idConsu + "+++";
										//Message byte[] a envoyer
										msgConsuTosend = msgToConsuString.getBytes();
										//Longueur du message a envoyer
										longueurMsgTosend = msgToConsuString.length();
										//Envoi le message de taille longueurMsgTosend
										outputStream.write(msgConsuTosend,0,longueurMsgTosend);
										outputStream.flush();
										break;
				//----------------------------------------------------------------------------------------						
									//Consultation d'un refus d'amitie
									case "NOKRF+++":
										//Message String a envoyer
										msgToConsuString = "NOFRI " + idConsu + "+++";
										//Message byte[] a envoyer
										msgConsuTosend = msgToConsuString.getBytes();
										//Longueur du message a envoyer
										longueurMsgTosend = msgToConsuString.length();
										//Envoi le message de taille longueurMsgTosend
										outputStream.write(msgConsuTosend,0,longueurMsgTosend);
										outputStream.flush();
										break;
				//----------------------------------------------------------------------------------------						
									//Consultation d'un flood
									case "FLOO?":
										//Message String a envoyer
										msgToConsuString = "OOLF> " + idConsu + " " + caseConsuString.substring(6, caseConsuString.length()-3) + "+++";
										//Message byte[] a envoyer
										msgConsuTosend = msgToConsuString.getBytes();
										//Longueur du message a envoyer
										longueurMsgTosend = msgToConsuString.length();
										//Envoi le message de taille longueurMsgTosend
										outputStream.write(msgConsuTosend,0,longueurMsgTosend);
										outputStream.flush();
										break;
				//----------------------------------------------------------------------------------------
									//En cas de message inconnu
									default:
										System.out.println("ERREUR - Consultation inconnue"); //ERREUR
										msgToSend = "NOCON+++";
										//On envoi le message sous forme de tableau de byte[]
										msgToSendByte = msgToSend.getBytes();
										outputStream.write(msgToSendByte);
										outputStream.flush();
										break;
				//----------------------------------------------------------------------------------------
								}
								
								break;
							}catch(Exception e) {
								// e.printStackTrace(); //DEBUG
								System.out.println("ERREUR - Une erreur est survenue lors de la connection"); //ERREUR
								msgToSend = "NOCON+++";
								//On envoi le message sous forme de tableau de byte[]
								msgToSendByte = msgToSend.getBytes();
								outputStream.write(msgToSendByte);
								outputStream.flush();
								break;
							}
	//--------------------------------------------------------------------------------------------------------------------------------------
						//En cas de message inconnu
						default:
							debugPrint("Message Inconnu"); //DEBUG
							break;
	//--------------------------------------------------------------------------------------------------------------------------------------
					}	//Fin switch
				}
			} catch(NullPointerException e) {
				debugPrint("Une connection a ete coupee prematurement par un Utilisateur"); //DEBUG
				// e.printStackTrace(); //DEBUG
			}
			catch(SocketException e) {
				//Permet de fermer ensuite la socket si le Client est arrete de force
			}
			catch(NegativeArraySizeException e) {
				//Permet de fermer ensuite la socket si on recoit un message de taille negatif (-1)
			}

			//Fermeture de l'InputStream, de l'OutputStream et de la Socket
			inputStream.close();
			outputStream.close();
			this.socket.close();
			debugPrint("Un client a ete deconnecte"); //DEBUG
		} catch(Exception e) {
			// e.printStackTrace(); //DEBUG
		}
	}

	//Fonction qui decode l'entier recu sur 2 octets
	public int decodeMdp(byte premier, byte deuxieme) {
		int mdp1 = (int)premier & 0xFF;
		int mdp2 = (int)deuxieme & 0xFF;
		return mdp2*256+mdp1;
	}

	//Cree un String en prenant les valeurs du tableau de byte de debut a offset(fin)
	public String byteToString(byte[] messageByte,int debut, int offset) {
		return new String(messageByte,debut,offset);
	}
}