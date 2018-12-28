package userinterface;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;


public class client {
	private static int portnumber = 81;
	private static Socket soc;
	private static  DataInputStream dis;
	private static  DataOutputStream dos;
	private static String ipAddress;
	public client(String ipAddress) {
		if (!connectToServer(ipAddress)) {
    		System.out.println("failed to open server " + ipAddress);            
    	}
	}
	private static boolean connectToServer(String ipAddress) {
		try { // open a new socket to the server 
			
    		soc = new Socket(ipAddress,portnumber);
    		dos = new DataOutputStream(soc.getOutputStream());
    		dis = new DataInputStream(soc.getInputStream());
    		System.out.println("Connected to Server:" + soc.getInetAddress() 
    				+ " on port: " + soc.getPort());
    		System.out.println("from local address: " + soc.getLocalAddress() 
    				+ " and port: " + soc.getLocalPort());
    	} 
        catch (Exception e) {
        	System.out.println("Failed to Connect to the Server at port: " + portnumber);
        	System.out.println("    Exception: " + e.toString());	
        	return false;
        }
		return true;
	}
	private void send(String o) {//send to server to get frame rate and the size wanted
		try {
		    System.out.println("Sending the required resolution");
		    dos.writeUTF(o);
		  //  dos.flush();
		} 
	    catch (Exception e) {
		    System.out.println("XX. Exception Occurred on Sending:" +  e.toString());
		}
    }
	

    // method to receive 
    private void receive() 
    {
		//String o = null;
    	 // BufferedImage img=null;
    	
		try {
			System.out.println("About to receive an object...");
		  //  o = dis.readUTF();
			ObjectInputStream ois = new ObjectInputStream(soc.getInputStream());
			byte[]  buffer = (byte[])ois.readObject();
			FileOutputStream fos = new FileOutputStream("C:\\Users\\zozo\\Downloads\\conan.jpg");
			fos.write(buffer); 
            System.out.println("Image received!!!!"); 
		} 
	    catch (Exception e) {
		    System.out.println(" Exception Occurred on Receiving:" + e.toString());
		    e.printStackTrace();
		}
		//return buffer;
    }

    public static void main(String args[]) throws UnknownHostException, IOException 
    {
    		while(connectToServer(ipAddress) == true) {
    		client theApp = new client(ipAddress);
    	//	clientApplication app = new clientApplication();
		    theApp.send("1,300,294");
		    theApp.receive();   
		    System.out.println("End of Application.");
    		}
    }
	
}
