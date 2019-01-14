package com.example.namragill.connecteddevproject;

import android.content.ContentValues;
import android.database.sqlite.SQLiteDatabase;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import java.net.UnknownHostException;
import java.util.ArrayList;

/**
 * Created by namragill on 2018-11-14.
 */

public class DashFragment extends Fragment {


    private RecyclerView recyclerViewMain;
    public static ArrayList<Bitmap> images;
    private RecyclerView.LayoutManager mLayoutManager;
    private RecyclerViewAdapter adapter;
    ViewImage viewImage;
    private NotifyFragment notifyFragment = new NotifyFragment();
    Helper dbHelper;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.recycler_view,container,false);
        initiatecomponents(view);
        images  = new ArrayList<>();
        //dbHelper = new Helper(getActivity());
        return view;
    }

    private void initiatecomponents(View view) {
        recyclerViewMain = (RecyclerView) view.findViewById(R.id.rec_view);
        viewImage = new ViewImage();
        try {
            AsyncTaskGetImage getImages = new AsyncTaskGetImage(this);
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }
    }

    private void setGridLayoutManager() {
        recyclerViewMain.setHasFixedSize(true);
        mLayoutManager = new GridLayoutManager(getContext(), 2);
        recyclerViewMain.setLayoutManager(mLayoutManager);
        adapter = new GridLayoutAdapter(getActivity(), images);
    }

    private void setAdapter() {
        recyclerViewMain.setAdapter(adapter);
    }

    public void setImage(ArrayList<Bitmap> image) {
        this.images=image;
        setGridLayoutManager();
        setAdapter();
       // notifyFragment.populatelist("Server message: " + "images recieved");
       /* SQLiteDatabase db = dbHelper.getWritableDatabase();
        ContentValues values = new ContentValues();
        values.put(Helper.COLUMN_VALUE,"Server message: " + "images recieved");
        db.insert(Helper.TABLE_NAME_MES, "", values);*/
//        Toast.makeText(getActivity(),"Please check images",Toast.LENGTH_SHORT);

    }

    public ArrayList<Bitmap> getImage() {
        Log.d("Dash", String.valueOf(this.images));
        return this.images;
    }


}
