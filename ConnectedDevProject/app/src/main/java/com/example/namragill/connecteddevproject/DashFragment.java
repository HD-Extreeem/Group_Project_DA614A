package com.example.namragill.connecteddevproject;

import android.graphics.Bitmap;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.RecyclerView;
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
    private ArrayList<Bitmap> images = new ArrayList<>();
    private RecyclerView.LayoutManager mLayoutManager;
    private RecyclerViewAdapter adapter;
    private RecyclerViewDisplay display;


    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.recycler_view,container,false);
        initiatecomponents(view);
        return view;
    }

    private void initiatecomponents(View view) {
        recyclerViewMain = (RecyclerView) view.findViewById(R.id.rec_view);
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
    }
}
