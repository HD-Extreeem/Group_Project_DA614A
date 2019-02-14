package com.example.namragill.connecteddevproject;

import android.app.Activity;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;

import java.util.ArrayList;

/**
 * Created by namragill on 2018-11-14.
 */

public class NotifyFragment extends Fragment implements View.OnClickListener {

    public static ListView listView;
   public static ArrayAdapter arrayAdapter;
    protected static FragmentActivity mActivity;
    public static Context context;
    Helper myDB;
    Cursor data;
    static Button button;

    public static ArrayList<String> theList = new ArrayList<>();


    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_notification,container,false);
        initialise(view);
     //   mainActivity = new MainActivity();
            context =  super.getContext();
        myDB = new Helper(getActivity());
        populatelist();
        return view;
    }


    @Override
    public Context getContext() {
        return super.getContext();
    }

    public void populatelist() {
        //mainActivity = new MainActivity();'

            final ArrayList<String> theList = new ArrayList<>();
            data = myDB.getAllData();
            int message = data.getColumnIndex(Helper.COLUMN_VALUE);
        Log.d("Populate", String.valueOf(message));

            if ( data.getCount() == 0){
                Toast.makeText(getActivity(),"There is no data in the database", Toast.LENGTH_LONG).show();
            }
            else{
                while (data.moveToNext()){
                    theList.add(data.getString(message) );
                }

                arrayAdapter = new ArrayAdapter<>(getActivity(), android.R.layout.simple_list_item_1, theList);
                listView.setAdapter(arrayAdapter);


        }

    }

    private void initialise(View view) {
        listView = (ListView)view.findViewById(R.id.list_notify);
        button = (Button)view.findViewById(R.id.del);
        button.setOnClickListener(this);
     
    }
    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
      //  mContext = context;
        if (context instanceof Activity){
            mActivity = (FragmentActivity) context;
        }
    }

    @Override
    public void onClick(View v) {
        SQLiteDatabase db = myDB.getWritableDatabase();
        db.delete(Helper.TABLE_NAME_MES, null, null);
        populatelist();
    }
}
