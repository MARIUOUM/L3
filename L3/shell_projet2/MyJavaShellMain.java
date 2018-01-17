import java.util.Scanner;

public class MyJavaShellMain {
	
	public static void main(String args[]){
		System.out.println("Démarrage du shell Java...");
		MyJavaShell shell = new MyJavaShell("myshell@java");
		Scanner scan = new Scanner(System.in);
		String[] arguments;
		while(true){
			System.out.print(shell.getLongName());
			String line = scan.nextLine();
			shell.execute_command(line);
		}
	}
}
