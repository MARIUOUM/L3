public class Consu {
	private String id;
	private byte[] message;
	
	public Consu(String id, byte[] message,int debut, int fin) {
		super();
		this.id = id;
		byte[] stockmessage = new byte[fin-debut];
		for(int i = 0; i<stockmessage.length; i++){
			stockmessage[i] = message[i];
		}
		this.message = stockmessage;
	}

	//------------ GETTER ET SETTER ------------ 
	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public byte[] getMessage() {
		return message;
	}

	public void setMessage(byte[] message) {
		this.message = message;
	}
	//------------------------------------------------ 
}
