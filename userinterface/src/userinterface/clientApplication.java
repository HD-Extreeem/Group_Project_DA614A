package userinterface;

import java.awt.BorderLayout;
import java.awt.EventQueue;
import java.awt.FlowLayout;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.DataInputStream;
import java.io.DataOutput;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.net.Socket;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;

public class clientApplication extends JFrame {
	private static int portnumber = 88;
	private static Socket soc;
	private static  DataInputStream dis;
	private static  DataOutputStream dos;
	private static String ipAddress;
	private static JPanel contentPane;
	public clientApplication(String ipAddress) {
		if (!connectToServer(ipAddress)) {
    		System.out.println("failed to open server " + ipAddress);        
    		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    		setBounds(100, 100, 450, 300);
    		contentPane = new JPanel();
    		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
    		contentPane.setLayout(new BorderLayout(0, 0));
    		setContentPane(contentPane);
    			
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
	


	/**
	 * Launch the application.
	 */
	private void receive() 
    {
    	
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
    }
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {						
			public void run() {		
				try {						
					clientApplication application = new clientApplication(ipAddress);	
					application.send("1,300,294");
				    application.receive();
				    JFrame frame = new JFrame("Camera frame");
					  //  JPanel panel = new JPanel();
					    contentPane.setLayout(new FlowLayout());
					    JLabel label = new JLabel("This is a label!");
			            ImageIcon mg = new ImageIcon("C:\\Users\\zozo\\Downloads\\conan.jpg");
			            JLabel imageLabel = new JLabel(mg); 
			            JButton button = new JButton();
			            button.setText("Connect");
			           // panel.add(button);
			            frame.add(imageLabel);
			            contentPane.add(label);
			            frame.add(button);
			            frame.add(contentPane);
			            frame.setDefaultCloseOperation
			            (JFrame.EXIT_ON_CLOSE);
			            frame.setSize(300, 300);
			            frame.setVisible(true);
			            System.out.println("image displayed");
				} catch (Exception e) {
					e.printStackTrace();
				}
				}			
		});
		
	}

	/**
	 * Create the frame.
	 */

}
