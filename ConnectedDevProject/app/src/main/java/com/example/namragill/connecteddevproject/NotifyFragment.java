package com.example.namragill.connecteddevproject;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import java.util.ArrayList;

/**
 * Created by namragill on 2018-11-14.
 */

public class NotifyFragment extends Fragment  {

    public static ListView listView;
   public static ArrayAdapter arrayAdapter;
    MainActivity mainActivity;
    protected static FragmentActivity mActivity;
    public static Context context;

    public static ArrayList<String> theList = new ArrayList<>();


    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_notification,container,false);
        initialise(view);
     //   mainActivity = new MainActivity();
            context =  super.getContext();
       return view;
    }

    private void populate() {
        populatelist(null);
    }


    @Override
    public Context getContext() {
        return super.getContext();
    }

    public void populatelist(String message) {
        //mainActivity = new MainActivity();

            theList.add(message);
            Log.d("notiy", String.valueOf(mActivity));
            Log.d("notiy", String.valueOf(android.R.layout.simple_list_item_1));
            Log.d("notiy", String.valueOf(theList));
            arrayAdapter = new ArrayAdapter<>(mActivity, android.R.layout.simple_list_item_1, theList);
            Log.d("notiy", String.valueOf(arrayAdapter));
            listView.setAdapter(arrayAdapter);
            Log.d("notdhdjn", String.valueOf(listView));



    }

    private void initialise(View view) {
        listView = (ListView)view.findViewById(R.id.list_notify);
     
    }
    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
      //  mContext = context;
        if (context instanceof Activity){
            mActivity = (FragmentActivity) context;
        }
    }

}
