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

public class DashFragment extends Fragment {

    TextView textView;
    ImageView imageView;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_dashboard,container,false);
        initiatecomponents(view);
        return view;
    }

    private void initiatecomponents(View view) {
        imageView =(ImageView) view.findViewById(R.id.imageView);
        textView = (TextView)view.findViewById(R.id.maindashboard);
        textView = (TextView)view.findViewById(R.id.maindashboard);
        textView.setText(R.string.image_no);
    }
}
