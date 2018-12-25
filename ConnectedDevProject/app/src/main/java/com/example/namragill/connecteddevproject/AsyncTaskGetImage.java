package com.example.namragill.connecteddevproject;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.util.Base64;
import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayList;

/**
 * Created by hadideknache on 2018-12-01.
 */

public class AsyncTaskGetImage extends AsyncTask<Void,Void,ArrayList<Bitmap>> {
    private RecyclerViewDisplay viewDisplay;
    public AsyncTaskGetImage(RecyclerViewDisplay viewDisplay) throws UnknownHostException {
        this.viewDisplay = viewDisplay;
        execute();
    }

    @Override
    protected ArrayList<Bitmap> doInBackground(Void... voids) {
        //byte [] downloaded = new byte[0];
        ArrayList<Bitmap> images = new ArrayList<>();
        String encoded;

        try {
            Log.d("AsynctaskGetImages","Inside socket sending method");
            Socket socket = new Socket(InetAddress.getByName("192.168.20.250"),8080);
            PrintStream stream = new PrintStream(socket.getOutputStream());
            stream.println("/send");
            Log.d("AsynctaskGetImages","sent to server");
            //Works for java <--> android image transfer ONLY!
            /*ObjectInputStream ois = new ObjectInputStream(socket.getInputStream());
            for (int i = 0;i<10;i++){
                downloaded = (byte[]) ois.readObject();
                images.add(BitmapFactory.decodeByteArray(downloaded,0,downloaded.length));
                stream.println("");
            }*/


            BufferedReader buf = new BufferedReader(new InputStreamReader(socket.getInputStream()));


            for (int i = 0;i<10;i++) {

                encoded = buf.readLine().trim();
                Log.d("AsynctaskGetImages","Got "+encoded.substring(encoded.length()-7,encoded.length()-1));
                //int maxLogSize = 4000;
                /*for(int j = 0; j <= encoded.length() / maxLogSize; j++) {
                    int start = j * maxLogSize;
                    int end = (j+1) * maxLogSize;
                    end = end > encoded.length() ? encoded.length() : end;
                    Log.v("AsynctaskGetImages", encoded.substring(start, end));
                }*/
                Log.d("AsynctaskGetImages","Got size = "+encoded.length());
                while(encoded.charAt(encoded.length()-1)!='Z' && encoded.charAt(encoded.length()-1)!='='){
                    encoded = encoded.substring(0,encoded.length()-1);
                }
                try{
                    byte [] decoded = Base64.decode(encoded, Base64.DEFAULT);
                    //Log.v("AsyncTaskGetImage", String.valueOf(count));
                    Log.d("AsynctaskGetImages","Sent"+i);
                    Bitmap map = BitmapFactory.decodeByteArray(decoded, 0, decoded.length);
                    images.add(map);
                } catch (IllegalArgumentException e){

                    e.printStackTrace();
                }




            }

            socket.close();


        } catch (IOException e) {
            e.printStackTrace();
        }
        return images;
    }

    @Override
    protected void onPostExecute(ArrayList<Bitmap> images) {
        super.onPostExecute(images);
        viewDisplay.setImage(images);
    }
}