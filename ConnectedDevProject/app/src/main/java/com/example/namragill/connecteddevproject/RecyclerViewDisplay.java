package com.example.namragill.connecteddevproject;

import android.graphics.Bitmap;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.RecyclerView;

import java.util.ArrayList;

/**
 * Created by hadideknache on 2018-12-01.
 * not Used
 */

public class RecyclerViewDisplay extends AppCompatActivity {
    private RecyclerView recyclerViewMain;
    private ArrayList<Bitmap> images = new ArrayList<>();
    private RecyclerView.LayoutManager mLayoutManager;
    private RecyclerViewAdapter adapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.recycler_view);

        recyclerViewMain = (RecyclerView) findViewById(R.id.rec_view);
       /* try {
           // AsyncTaskGetImage getImages = new AsyncTaskGetImage(this);
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }*/
    }

    private void setGridLayoutManager() {
        recyclerViewMain.setHasFixedSize(true);
        mLayoutManager = new GridLayoutManager(this, 2);
        recyclerViewMain.setLayoutManager(mLayoutManager);
        adapter = new GridLayoutAdapter(this, images);
    }

    private void setAdapter() {
        recyclerViewMain.setAdapter(adapter);
    }

    public void setImage(ArrayList<Bitmap> image) {
        this.images=image;
        setGridLayoutManager();
        setAdapter();
    }
}
