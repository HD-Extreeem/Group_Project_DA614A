package userinterface;


import java.awt.Canvas;
import java.awt.EventQueue;
import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.net.Socket;

import javax.imageio.ImageIO;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

import java.awt.BorderLayout;

public class Panel extends JPanel {

	/**
     *
     */
    private static final long serialVersionUID = 1L;
    private JFrame frame;
	private static Socket soc;
	private static String ipAddress = "192.168.20.252";
	private static int portnumber = 88;
	protected BufferedImage image;
   public static JPanel panel;
   public static int flag = 0;
   
    
	/**
 	* Launch the application.
 	*/
 /*   public static void main(String[] args) {
    	EventQueue.invokeLater(new Runnable() {
        	public void run() {
            	try {
                	soc = new Socket(ipAddress,portnumber);
                	Panel window = new Panel();
                	window.frame.setVisible(true);
            	} catch (Exception e) {
                	e.printStackTrace();
            	}
        	}
    	});
	}*/

	/**
 	* Create the application.
 	* @throws IOException
 	*/
	public Panel() throws IOException {
    	super();
   	// initialize();
   	 
	}

	/**
 	* Initialize the contents of the frame.
	 * @param newImage 
 	* @throws IOException
 	*/
	public void initialize(BufferedImage image, boolean newImage) throws IOException {
	
	this.image = image; 	 
      JFrame frame = new JFrame();
	frame.getContentPane().add(this, BorderLayout.CENTER);
    
    ImageIcon icon = new ImageIcon(image);
    JLabel label = new JLabel(icon);
	
	if(newImage == true) {
		frame.remove(label);
		label.setVisible(false); 	
		icon = new ImageIcon(image);
		label = new JLabel(icon);
	}
   
   	this.add(label);
   	frame.setDefaultCloseOperation
        	(JFrame.EXIT_ON_CLOSE);
    frame.pack();
    frame.setVisible(true);
    frame.revalidate();
    frame.repaint();
      
      
   	 
 		 
   	 
	}
  

}



