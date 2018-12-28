package userinterface;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class main {
	private static int portnumber = 88;
	
	public static void main(String args[]) throws IOException {
		boolean listen =true;
		ServerSocket soc;
		soc = new ServerSocket(portnumber);
		System.out.println("server started:"+portnumber);
		while(listen) {
			Socket clientsocket;
			System.out.println("server started to listen");
			clientsocket = soc.accept();
			System.out.println("connection accepted"+clientsocket.getInetAddress().toString()+"for the port"+clientsocket.getPort());
			
			clientHandler handler = new clientHandler(clientsocket);
			handler.start();
			//System.out.println("connection stopped");
			
		}
	}
}
