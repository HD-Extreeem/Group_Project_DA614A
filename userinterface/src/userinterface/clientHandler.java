package userinterface;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.awt.Color;
import java.awt.Image;
import java.awt.image.BufferedImage;
import javax.imageio.*;
import javax.swing.ImageIcon;

import org.omg.CORBA.portable.ApplicationException;


public class clientHandler extends Thread {
private Socket soc;
private DataInputStream dis;
private DataOutputStream dos;

private ObjectInputStream ois;
Image newimg;
static BufferedImage bimg;
public clientHandler(Socket clientSocket) {
	this.soc = clientSocket;
}
@Override
	public void run() {
		
			try {
				this.dis = new DataInputStream(soc.getInputStream());
				this.dos = new DataOutputStream(soc.getOutputStream());
				while(this.readCommand()) {}
			} 
		catch( Exception e) {}
}

private boolean readCommand() {
    String s = null;
//	byte [] b = null;
	
    try {
        s = dis.readUTF();
    //	b = (byte[]) ois.readObject();
        System.out.println("reading");
     //   ois.close();
    //   dis.close();
    } 
    catch (Exception e){    
    	this.closeSocket();
    	System.out.println("closing it");
        return false;
    }
    System.out.println("Received a String object from the client (" + s+ ").");

	    if (s.equalsIgnoreCase("300,500")){ 
	    	this.send();
    }      
//    if(b.equals("buffer")) {
//    	this.send();
//    }
    else { 
       this.sendError("Invalid command: " + s); 
    }
	return true;
    
}
//Send to the client 
private void send() {
	String o = "hi from server";
    try {
   //     System.out.println("02. -> Sending (" + o +") to the client.");
    	System.out.println("sending object");
        this.dos.writeUTF(o);
      //  this.dos.flush();
    	//int width = 300;
    	//int height = 294;
    	FileInputStream fis = new FileInputStream("C:\\Users\\zozo\\Desktop\\conan.jpg");
    	byte[] 	buffer = new byte[fis.available()];
        fis.read(buffer);
        ObjectOutputStream  oos = new ObjectOutputStream(soc.getOutputStream()) ;
        oos.writeObject(buffer); 
        System.out.println("Image sent!!!!");
    } 
    catch (Exception e) {
    	e.printStackTrace();
    }

}
public byte[] scale(byte[] fileData, int width, int height) {
    ByteArrayInputStream in = new ByteArrayInputStream(fileData);
    try {
        BufferedImage img = ImageIO.read(in);
        Image scaledImage = img.getScaledInstance(width, height, Image.SCALE_SMOOTH);
        BufferedImage imageBuff = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
        imageBuff.getGraphics().drawImage(scaledImage, 0, 0, new Color(0,0,0), null);
        ByteArrayOutputStream buffer = new ByteArrayOutputStream();
        ImageIO.write(imageBuff, "jpg", buffer);
        System.out.println("image scaled");
        return buffer.toByteArray();
    } catch (IOException e) {
        try {
			throw new ApplicationException("IOException in scale", null);
		} catch (ApplicationException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
    }
	return fileData;
}
public void sendError(String message) { 
  //  this.send(message);
}

// Close the client socket 
public void closeSocket() { 
    try {
        
        this.soc.close();
    } 
    catch (Exception e) {
    	e.printStackTrace();
    }
}
}
