package userinterface;

import java.awt.BorderLayout;
import java.awt.EventQueue;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.ObjectInputStream;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Base64;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;
import java.awt.Color;

import javax.imageio.ImageIO;
import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.SwingUtilities;
import sun.misc.BASE64Decoder;


public class Application extends JFrame {

	private JPanel contentPane;
	private byte[] imgBuf;
	private static int portnumber = 8888;
	private static Socket soc;
	public static  DataInputStream dis;
	public static String ipAddress = "192.168.20.252";
	private static PrintStream sendStream;
	private static BufferedInputStream recstream;
	private static BufferedImage img;
	private static JLabel lblNewLabel;
	private static   int length = 40000;
	public static Panel panel;
	public static String resolution;

	/**
 	* Launch the application.
 	*/
	private static boolean connectToServer(String ipAddress) {
   	 
    	try { // open a new socket to the server        	 
        	soc = new Socket(ipAddress,portnumber);
        	sendStream = new PrintStream(soc.getOutputStream());
        	dis = new DataInputStream(soc.getInputStream());
        	System.out.println("Connected to Server:"
                	+ " on port: " + soc.getPort());
        	System.out.println("from local address: "
                	+ " and port: " + soc.getLocalPort());
        	lblNewLabel = new JLabel();       	 
    	}
    	catch (Exception e) {
        	System.out.println("Failed to Connect to the Server at port: " + portnumber);
        	System.out.println("	Exception: " + e.toString());    
        	return false;
    	}
    	return true;
	}
	private void send(String o) {//send to server to get frame rate and the size wanted
    	try {
    	//	while(o!=null) {
    		this.resolution = o;
        	System.out.println("Sending the required resolution " + o);
        	sendStream.println(o); 
    	//	}
    	}
    	catch (Exception e) {
        	System.out.println("XX. Exception Occurred on Sending:" +  e.toString());
    	}
	}
    
	private void receive()
    
	{
   	 
   	  String encoded;
   	  boolean newImage = false;
   	  int first = 0;
    	try {
        	recstream = new BufferedInputStream(soc.getInputStream());
        	System.out.println("About to receive an object...");
        	BufferedReader buf = new BufferedReader(new InputStreamReader(soc.getInputStream()));
        	//PrintWriter	out = new PrintWriter(soc.getOutputStream(), true);
      	  System.out.println("server says"+ buf.readLine().trim());
        	encoded = buf.readLine().trim();
           	System.out.println("AsynctaskGetImages"+"Got size = "+encoded);
           	
           BASE64Decoder decoder = new BASE64Decoder();
       //   send(resolution);
       	System.out.println("AsynctaskGetImages"+"Got size = "+encoded.length());
        	while(encoded.charAt(encoded.length()-1)!='Z' && encoded.charAt(encoded.length()-1)!='='){
            	encoded = encoded.substring(0,encoded.length()-1);
            	  System.out.println("Namra says"+ encoded);
        	}
        	try{
            	byte [] decode = decoder.decodeBuffer(encoded);
            	ByteArrayInputStream bis = new ByteArrayInputStream(decode);
            	img = ImageIO.read(bis);
            	if (img != null){
                   	panel = new Panel();
              		 panel.initialize(img,newImage);
                    newImage = true;
                   	System.out.println("sooon view image");   
                	img = null;
                  	 
               	}
        	} catch (IllegalArgumentException e){

            	e.printStackTrace();
}
        	

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
                	Application frame = new Application(ipAddress);
                	//frame.send("300");
               	// frame.receive();
                	frame.setVisible(true);
                	frame.repaint();
            	} catch (Exception e) {
                	e.printStackTrace();
            	}
        	}
       	 
    	});
	}

	/**
 	* Create the frame.
 	*/
	public Application(String ipAddress) {
    	Application.ipAddress = ipAddress;
    	if (!connectToServer(ipAddress)) {
        	System.out.println("failed to open server " + ipAddress);
    	}
   	 
    	setDisplayLabel();
   	 
}
    
	private void setDisplayLabel() {
    	// TODO Auto-generated method stub
    	setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    	setBounds(100, 100, 450, 300);
    	contentPane = new JPanel();
    	contentPane.setBackground(Color.WHITE);
    	contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
    	contentPane.revalidate();
    
    	setContentPane(contentPane);    
   	 
    	String[] options = {"800,450","250,330"};
    	final JComboBox comboBox = new JComboBox(options);
   	 
    	// JLabel lblNewLabel = new JLabel();
    
   	 
    	JButton btnNewButton = new JButton("Send");
    	btnNewButton.addActionListener(new ActionListener() {
        	public void actionPerformed(ActionEvent e) {
        		
            	String str = (String)comboBox.getSelectedItem();       
            	send(str);           	
            	receive();           	 
        	}
    	});
    
	//	btnNewButton.addActionListener(new ButtonListener());
    	System.out.print("setting image");
   	 
    	GroupLayout gl_contentPane = new GroupLayout(contentPane);
    	gl_contentPane.setHorizontalGroup(
        	gl_contentPane.createParallelGroup(Alignment.LEADING)
            	.addGroup(gl_contentPane.createSequentialGroup()
                	.addContainerGap()
                	.addGroup(gl_contentPane.createParallelGroup(Alignment.LEADING)
                    	.addGroup(gl_contentPane.createSequentialGroup()
                        	.addGap(10)
                        	.addComponent(btnNewButton, GroupLayout.PREFERRED_SIZE, 98, GroupLayout.PREFERRED_SIZE))
                    	.addGroup(gl_contentPane.createSequentialGroup()
                        	.addComponent(lblNewLabel, GroupLayout.PREFERRED_SIZE, 319, GroupLayout.PREFERRED_SIZE)
                        	.addGap(18)
                        	.addComponent(comboBox, GroupLayout.PREFERRED_SIZE, 73, GroupLayout.PREFERRED_SIZE)))
                	.addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
    	);
    	gl_contentPane.setVerticalGroup(
        	gl_contentPane.createParallelGroup(Alignment.LEADING)
            	.addGroup(gl_contentPane.createSequentialGroup()
                	.addGroup(gl_contentPane.createParallelGroup(Alignment.LEADING)
                    	.addGroup(gl_contentPane.createSequentialGroup()
                        	.addGap(28)
                        	.addComponent(lblNewLabel, GroupLayout.PREFERRED_SIZE, 163, GroupLayout.PREFERRED_SIZE)
                        	.addPreferredGap(ComponentPlacement.UNRELATED)
                        	.addComponent(btnNewButton, GroupLayout.PREFERRED_SIZE, 40, GroupLayout.PREFERRED_SIZE))
                    	.addGroup(gl_contentPane.createSequentialGroup()
                        	.addGap(40)
                        	.addComponent(comboBox, GroupLayout.PREFERRED_SIZE, 22, GroupLayout.PREFERRED_SIZE)))
                	.addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
    	);
    	contentPane.setLayout(gl_contentPane);
   	 
	}
  	}


