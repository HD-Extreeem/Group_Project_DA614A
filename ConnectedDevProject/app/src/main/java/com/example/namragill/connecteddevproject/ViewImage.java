package com.example.namragill.connecteddevproject;

import android.graphics.Bitmap;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.ImageView;

import java.util.ArrayList;

public class ViewImage extends AppCompatActivity {
    int pos;
    ImageView imageView;
    private static ArrayList<Bitmap> img;
    private Bitmap image;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_view_image);
        imageView  = (ImageView)findViewById(R.id.showImg);
        Log.d("View", String.valueOf(imageView));

        imageView.setImageBitmap(img.get(1));
        Log.d("View","pic showing");

    }


    public void setImage(ArrayList<Bitmap> image, int pos) {
        Log.d("View","pic gotten");
        this.img = image;
        Log.d("View", String.valueOf(image));
        this.pos = pos;
        Log.d("View", String.valueOf(pos));
    }
}
