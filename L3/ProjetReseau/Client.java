import java.net.*;
import java.io.*;
import java.util.*;

public class Client {

	private int debugOption;	//Option permettant de choisir le debug au lancement
	private ClientTCP clientTCP;
	
	//Instanciation des options du serveur auquel on se connecte
	private int portTCPServer = 3539;
	private String adressTCPServer;
	
	//Instanciation des elements de communications avec le serveurs
	private Socket socket;
	private InputStream inputStream;
	private OutputStream outputStream;
	private boolean isOpened = false;

	//Instanciation des elements de communication UDP
	private Integer portUDP;
	private DatagramSocket dso;

	//Instanciation pour éviter les nombres magiques permettant les tests
	private final int testPortUDPmin = 1024;
	private final int testPortUDPmax = 9999;
	private final int testMdpMin = 0;
	private final int testMdpMax = 65535;
	private final int testLengthIdUser = 8;

	public Client(int debugOption) {
		this.debugOption = debugOption;
		this.clientTCP = new ClientTCP(this);
		this.portUDP = null; //Le port est initialisé à null, il sera modifié à la connection.
		//On récupère l'adresse locale en supposant que le serveur s'y trouve également si l'on ne donne pas d'adresse
		try {
			this.adressTCPServer = InetAddress.getLocalHost().getHostAddress();
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	public Client(String adressTCPServer, int portTCPServer, int debugOption) {
		this.debugOption = debugOption;
		this.clientTCP = new ClientTCP(this);
		this.portUDP = null; //Le port est initialisé à null, il sera modifié à la connection.
		//On récupère l'adresse locale en supposant que le serveur s'y trouve également si l'on ne donne pas d'adresse		
		this.adressTCPServer = adressTCPServer;
		this.portTCPServer = portTCPServer;
	}
	
	//Methode permettant d'afficher les DEBUG de base
	public void debugPrint(String msgDebug) {
		if (this.debugOption > 0) {
			System.out.println("DEBUG - " + msgDebug);
		}
	}


	//Lancement du Thread d'écoute UDP
	public void listenUDP() {
		try {
			this.dso = new DatagramSocket(this.portUDP);
			ClientUDP clientUDP = new ClientUDP(this, this.dso);
			Thread threadClientUDP = new Thread(clientUDP);
			threadClientUDP.start();
			debugPrint("Lancement du port d'écoute ClientUDP (Thread lancé)"); //DEBUG
		} catch(Exception e) {
			e.printStackTrace(); //DEBUG
		}
	}

	//Initialisation de la socket de connection, du Reader et du Writer
	public void connectServer() {
		try {
			this.socket = new Socket(this.adressTCPServer, this.portTCPServer);
			this.inputStream = socket.getInputStream();
			this.outputStream = socket.getOutputStream();
			this.isOpened = true;
			debugPrint("Connection TCP au Serveur"); //DEBUG
		} catch(Exception e) {
			// e.printStackTrace(); //DEBUG
		}
	}

	//Fermeture de la socket de connection, du Reader et du Writer
	public void disconnectServer() {
		try {            
			this.outputStream.close();
			this.inputStream.close();
			this.socket.close();
			this.isOpened = false;
			System.out.println("Vous êtes maintenant déconnecté, à bientôt!\n");
		} catch(Exception e) {
			// e.printStackTrace(); //DEBUG
		}
	}
	
	//Fermeture du DatagramSocket
	public void stopUDP(){
		try{
			this.dso.close();
		} catch (NullPointerException e) {
			e.printStackTrace(); //DEBUG
		}
	}

	//Demande au client l'action qu'il souhaite effectuer lorsqu'il est déconnecté
	public void choiceActionDisconnected() {
		try{
			//Demande de l'action au client
			System.out.println("Veuillez séléctionner l'action à effectuer.");
			System.out.print("1: Connexion\n2: Inscription\nVotre choix: ");
			Scanner sc = new Scanner(System.in);
			int action = sc.nextInt();
			sc.nextLine(); //On vide la ligne (pour pouvoir utiliser nextLine() après un nextInt())
			System.out.println(); //SEPARATEUR

			//Initialisation des attributs du Client qui seront récupérés
			String id;
			int mdp = -1;
			int portUDP = -1;

			//On traite les actions du client deconnecte
			switch(action) {
				//Connexion
				case 1:
					//On demande à l'utilisateurs son id et son mdp
					System.out.print("Identifiant: ");
					id = sc.nextLine();
					System.out.print("Mot de passe: ");
					mdp = sc.nextInt();
	
					//On instancie la Socket, le Reader et le Writer s'ils ne le sont pas déjà
					if (!this.isOpened) {
						connectServer();
					}
	
					//On se connecte au réseau social et on lance l'écoute des notifications, puis on propose les actions connectées
					if (this.clientTCP.connect(id, mdp, socket, outputStream, inputStream)) {
						System.out.println("Connexion établie, vous recevez à présent les notifications.\n");
						this.listenUDP();
						choiceActionConnected();
					} else {
						System.out.println("ERREUR - Identifiant ou mot de passe incorect! Merci de réessayer.\n"); //ERREUR
						disconnectServer(); //On se déconnecte du serveur
						choiceActionDisconnected();
					}
					break;
	
				//Inscription
				case 2:
					//On demande à l'utilisateurs son id qui doit être un certains nombre de caractères
					System.out.print("Identifiant: ");
					id = sc.nextLine();
					while(id.length() != testLengthIdUser) {
						System.out.print("ERREUR - L'identifiant doit faire " + testLengthIdUser + "caractères alphanumériques, veuillez réessayer: ");
						id = sc.nextLine();
					}
					
					//On demande à l'utilisateurs son mdp
					try {
						System.out.print("Mot de passe (entier entre 0 et 65 535) ");
						mdp = sc.nextInt();
					} catch(InputMismatchException e) {
						System.out.print("le mot de passe doit etre un entier\n");
					}
	
					//Si le mdp est trop grand, negatif ou pas un entier on redemande le mdp
					//Valeur de base du mdp = -1
					while(mdp>65535 || mdp<0 ) {
						sc.nextLine(); //On vide la ligne (pour pouvoir utiliser nextLine() après)
						try {
							System.out.print("Mot de passe (entier entre 0 et 65 535) ");
							mdp = sc.nextInt();
						} catch(InputMismatchException e) {
							System.out.print("le mot de passe doit etre un entier\n");
						}
					}
					//Premiere demande du port UDP
					try {
						System.out.print("Port UDP (entre 1024 et 9999) : ");
						portUDP = sc.nextInt();
					} catch(InputMismatchException e) {
						System.out.print("le portUDP doit etre un entier inferieur a 9999 et superieur a 1024\n");
					}

					//Si le mdp est trop grand, negatif ou pas un entier on redemande le mdp
					//Valeur de base du mdp = -1
					while(portUDP>9999 || portUDP<1024 ) {
						sc.nextLine(); //On vide la ligne (pour pouvoir utiliser nextLine() après)
						try {
							System.out.print("Port UDP (entre 1024 et 9999) : ");
							portUDP = sc.nextInt();
						} catch(InputMismatchException e) {
							System.out.print("le portUDP doit etre un entier inferieur a 9999 et superieur a 1024\n");
						}
					}
					sc.nextLine(); //On vide la ligne (pour pouvoir utiliser nextLine() après)
	
					//On instancie la Socket, le Reader et le Writer s'ils ne le sont pas déjà
					if (!this.isOpened) {
						connectServer();
					}
					
					if (this.clientTCP.register(id, mdp, portUDP, socket, outputStream, inputStream)) {
						System.out.println("Inscription réussie, vous recevez a present les notifications.\n");
						this.listenUDP();
						choiceActionConnected();
					} else {
						System.out.println("ERREUR: Cet identifiant ou ce port existe déjà! Merci de réessayer.\n"); //ERREUR
						disconnectServer(); //On se déconnecte du serveur
						choiceActionDisconnected();
					}
					break;
				//En cas d'erreur
				default:
					System.out.println("ERREUR: Action inconnue, merci de réessayer correctement."); //ERREUR
					choiceActionDisconnected();
					break;
			}
			//Fermeture du Scanner
			sc.close();
		}catch(InputMismatchException e) {
			System.out.println("Entrez un entier entre 1 et 2");
			choiceActionDisconnected();
		}
	}

	//Demande au client l'action qu'il souhaite effectuer lorsqu'il est connecté
	public void choiceActionConnected() {
		try{
			System.out.println("Veuillez séléctionner l'action à effectuer.");
			System.out.print("1: Consulter les notifications.\n2: Envoyer un message à un ami.\n3: Ajouter un nouvel ami.\n4: Obtenir la liste des utilisateurs.\n5: Envoyer un message à tous vos amis.\n6: Se déconnecter\nVotre choix: ");
			Scanner sc = new Scanner(System.in);
			int action = sc.nextInt();
			System.out.println(); //SEPARATEUR
			sc.nextLine();	//On vide la ligne (pour pouvoir utiliser nextLine() après un nextInt())	//++

			//On traite les actions de l'utilisateur connecte
			switch(action) {
				//Consultation les notifications
				case 1:
					this.clientTCP.consultation(socket,outputStream,inputStream);
					choiceActionConnected();
					break;
	
				//Envoyer un message à un ami 
				case 2:
					//On vérifie que le message ne contient pas "+++"
					System.out.println("Tapez votre message:");
					String msgForFriend = sc.nextLine();
					while(msgForFriend.contains("+++") || msgForFriend.length() == 0) {
						System.out.println("Votre message ne doit pas être vide et ne doit pas contenir de '+++', merci de réessayer:");
						msgForFriend = sc.nextLine();
					}
					
					//On demande à quel ami l'utilisateur souhaite l'envoyer
					System.out.println("À qui souhaitez-vous envoyer un message?");
					String friendIdToSendMsg = sc.nextLine();
	
					if(this.clientTCP.sendMessage(msgForFriend, friendIdToSendMsg, socket, outputStream, inputStream)) {
						System.out.println("Message envoyé.");
						choiceActionConnected();
					} else {
						System.out.println("ERREUR - Vous n'êtes pas encore ami avec " + friendIdToSendMsg);
						choiceActionConnected();
					}
					break;
	
				//Ajouter un nouvel ami
				case 3:
					//On demande à l'utilisateurs son id qui doit être un certains nombre de caractères
					System.out.print("Entrez l'identifiant de l'ami que vous souhaitez ajouter: ");
					String demandeAmi = sc.nextLine();
					System.out.println(); //SEPARATEUR
					while(demandeAmi.length() != testLengthIdUser) {
						System.out.print("ERREUR - L'identifiant doit faire " + testLengthIdUser + "caractères alphanumériques, veuillez réessayer: "); //ERREUR
						demandeAmi = sc.nextLine();
						System.out.println(); //SEPARATEUR
					}
					//Tentative d'ajout d'ami
					this.clientTCP.addFriend(demandeAmi,socket,outputStream,inputStream);
					//Peu importe le resultat on reviens dans le menu
					choiceActionConnected();
					break;
	
				//Obtenir la liste des utilisateurs
				case 4:
					ArrayList<String> listUsers = this.clientTCP.getListUsers(socket, outputStream, inputStream);
					System.out.println("Voici la liste des utilisateurs:");
					for (String user: listUsers) {
					    System.out.println(user);
					}
					System.out.println(); //SEPARATEUR
					choiceActionConnected();
					break;
	
				//Envoyer un message à tous les utilisateurs
				case 5:
					System.out.println("Entrez votre message de Flood :");
					String msgFlood = sc.nextLine();
					if(this.clientTCP.flood(msgFlood,socket,outputStream,inputStream)) {
						System.out.println("Message transmis a tout vos amis.");
					}else {
						System.out.println("ERREUR - Le message n'a pas été transmis aux amis."); //ERREUR
					}
					choiceActionConnected();
					break;
	
				//Se déconnecter
				case 6:
					if(this.clientTCP.disconnect(socket,outputStream,inputStream)) {
						stopUDP();
						debugPrint("Fermeture du port d'ecoute ClientUDP (Thread arreté)"); //DEBUG
						choiceActionDisconnected();
					}
					else {
						System.out.println("ERREUR - Deconnection echoue cote client\nFermeture des socket/input/output de force"); //ERREUR
						disconnectServer();
					}
					break;
	
				//Erreur
				default:
					System.out.println("ERREUR - Action inconnue, merci de réessayer correctement."); //ERREUR
					choiceActionConnected();
					break;
			}
			//Fermeture du Scanner
			sc.close();
		} catch(InputMismatchException e) {
			System.out.println("Entrez un entier entre 1 et 6");
			choiceActionConnected();
		}
	}


	public static void main(String[] args) {
		//Création du client vide et de l'option debug
		Client client;
		int debugOption = 0;
		
		//Lorsqu'on spécifie l'adresse et le port à la compilation
		if (args.length >= 2) {
			//Lorsqu'on rajoute l'option debug
			if (args.length == 3) {
				debugOption = Integer.parseInt(args[2]);
			}
			client = new Client(args[0], Integer.parseInt(args[1]), debugOption);
		} else {
			//Lorsqu'on rajoute l'option debug
			if (args.length == 1) {
				debugOption = Integer.parseInt(args[0]);
			}
			//Sinon on se connecte avec les paramètres du serveurs de base, en local
			client = new Client(debugOption);
		}
		System.out.print("Bienvenue! ");
		client.choiceActionDisconnected();
	}
	
	//------------ GETTER ET SETTER ------------ 
	public int getDebugOption() {
		return this.debugOption;
	}

	public void setPortUDP(int portUDP) {
		this.portUDP = portUDP;
	}

	public int getPortUDP() {
		return this.portUDP;
	}
	//------------------------------------------------ 
}