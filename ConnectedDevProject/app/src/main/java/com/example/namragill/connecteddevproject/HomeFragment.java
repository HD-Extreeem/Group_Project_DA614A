package com.example.namragill.connecteddevproject;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

/**
 * Created by namragill on 2018-11-14.
 */

public class HomeFragment extends Fragment {

    ImageView imageView;
    TextView textView;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {


        View view = inflater.inflate(R.layout.fragment_home,container,false);
        initiatecomponents(view);
        return view;
    }

    private void initiatecomponents(View view) {
        imageView =(ImageView) view.findViewById(R.id.mainimg);
        imageView.setImageResource(R.drawable.thumbsupicon);
        textView = (TextView)view.findViewById(R.id.mainhome);
        textView.setText(R.string.recent_good);
    }
}
