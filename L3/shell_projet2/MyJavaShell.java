import java.io.File;
import java.io.FileFilter;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.regex.Pattern;

public class MyJavaShell {

	private String name;
	
	
	public MyJavaShell(String name){
		this.name = name;
	}

	public String getName(){
		return this.name;
	}
	
	/* 
	 * Methode qui donne le nom avec le chemin du repertoire courant
	 */
	public String getLongName(){
		return (this.name+":"+this.pwd()+"$ ");
	}
	
	/* 
	 * Retourne le chemin du repertoir courant
	 */
	public String pwd(){
		return ( System.getProperty("user.dir") );	
	}
	
	/* 
	 * Liste le repertoire courant
	 */
	public void ls(String dir){
		File directory;
		if(dir.equals("")){
			directory = new File(this.pwd()).getAbsoluteFile();
		} else {
			directory = new File(dir).getAbsoluteFile();
		}
		if(directory.exists()){
			File[] files = directory.listFiles();
			for(File file: files){
				if(file.isDirectory())
					System.out.print("D: ");
				else
					System.out.print("F: ");
				
				System.out.println(file.getName());
			}
		} else 
			System.out.println("Le répertoire \""+dir+"\" n'existe pas.");
		
	}
	
	/*
	 * Change le repertoir courant vers le repertoire dir
	 */
	public void cd(String dir){
		File directory;
		String currentDirectory = this.pwd();
		if(dir.equals("..")){
			String parent = new File(currentDirectory).getParent();
			if(parent == null)
				directory = new File(currentDirectory).getAbsoluteFile();
			else
				directory = new File(parent).getAbsoluteFile();
		}
		else {
			directory = new File(dir).getAbsoluteFile();
		}
		if(directory.exists())
			System.setProperty("user.dir", directory.getAbsolutePath());
		else
			System.out.println("Le répertoire \""+dir+"\" n'existe pas.");
	}
	
	/*
	 * Affiche la date au format donne ou au format par defaut
	 */
	public void date(String format){
		String form;
		SimpleDateFormat ft;
		if(format.equals("")){
			form = "yyyy-MM-dd";
			ft = new SimpleDateFormat(form);
		}
		else{
			if(format.startsWith("+")){
				String regex = "([yyyy]-[MM]-[dd])";
				form = format.substring(1,format.length());
				System.out.println(form);
				form = form.replace("%d", "dd");
				form = form.replace("%H", "hh");
				form = form.replace("%m", "mm");
				form = form.replace("%M", "MM");
				form = form.replace("%Y", "yyyy");
				System.out.println(form);
				if(Pattern.matches(regex,form)){
					ft = new SimpleDateFormat(form);
				}
				else{
					System.out.println("Format de la commande date invalide: date [<format>=+%Y-%M-%d]");
					return;
				}
			}
			else{
				System.out.println("L'argument de la commande doit commencer avec +: date [<format>=+%Y-%M-%d]");
				return;
			}
		}
		System.out.println("Date actuelle: "+ft.format(new Date()));
	}
	
	/*
	 * Recherche dans le repertoire donne tous elements remplissant les criteres (l'expression reguliere)
	 */
	public void find(String args[]){
		if( args[2].equals("-name") || args[2].equals("-iname") ){
			String path = args[1];
			String iname= args[2];
			String regex = args[3];
			System.out.println(path +" " +iname+ " " +regex);
			File[] listOfFiles = listFilesMatching(new File(path), regex);
			if(listOfFiles != null){
				for (int i = 0; i < listOfFiles.length; i++) {
					if (listOfFiles[i].isFile()) {
						System.out.println("File " + listOfFiles[i].getName());
					}
				}
			}
			System.out.println("Erreur expression réguliere de la commande find.");
		}
		else
			System.out.println("Argument -name ou iname manquant:\n find <chemin> -name <expr. reg.>\n find <chemin> -iname <expr. reg.> ");
	}
	
	/* 
	 * Lance un thread pour compter jusqu'a un entier donne
	 */
	public void compteJusqua(String args[]){
		//int entier = Integer.parseInt()
		/*if(args.length == 3){
			(new Thread(){
				@Override
				public 
			})
		} 
		else if(args.length == 2){
		
		} 
		else
		*/
	}
	
	/* 
	 * Execute la methode selon la commande tapee sur le shell
	 */
	public void execute_command(String s){
		if( s.length() == 0 ){
			
		}else {
			String[] args;
			if( s.contains(" ") ){
				args = s.split(" ");
			} else {
				args = new String[1];
				args[0] = s;
			}
			
			if( args[0].equals("pwd") ){
				System.out.println(pwd());
			}
			else if( args[0].equals("ls") ){
				if(args.length == 1)
					ls("");
				else
					System.out.println("La commande ls ne prend pas d'argument: ls");
			}
			else if( args[0].equals("cd") ) {
				if(args.length == 2)
					cd(args[1]);
				else if(args.length == 1)
					System.out.println("Argument manquant: cd [directory or path]");
				else
					System.out.println("La commande cd prend un argument au maximum: cd [directory or path]");
			}
			else if( args[0].equals("date") ) {
				if(args.length == 2)
					date(args[1]);
				else if(args.length == 1)
					date("");
				else
					System.out.println("La commande date prend un argument au maximum: date [<format>=+%Y-%m-%d]");
			}
			else if( args[0].equals("find") ) {
				if(args.length == 4)
					find(args);
				else
					System.out.println("La commande find prend 3 arguments:\n find <chemin> -name <expr. reg.>\n find <chemin> -iname <expr. reg.> ");
			}
			else {
				System.out.println("Commande invalide...");
			}
			
		}
	}
	
	public File[] listFilesMatching(File root, String regex) {
		if(!root.isDirectory()) {
			System.out.println(root+" n'est pas un repertoire");
		}
		final Pattern p = Pattern.compile(regex);
		return root.listFiles(new FileFilter(){
			@Override
			public boolean accept(File file) {
				return p.matcher(file.getName()).matches();
			}
		});
	}
}
