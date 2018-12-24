package com.example.namragill.connecteddevproject;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.app.NotificationCompat;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttMessage;

/**
 * Created by namragill on 2018-11-14.
 */

public class HomeFragment extends Fragment {

    ImageView imageView;
    public static TextView textStatus;
    MqttHelper mqttHelper;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {


        View view = inflater.inflate(R.layout.fragment_home, container, false);
        initiatecomponents(view);
        startMqtt();
        Log.d("Debug", "after method");

        return view;
    }
    private void initiatecomponents(View view) {
        imageView =(ImageView) view.findViewById(R.id.mainimg);
        imageView.setImageResource(R.drawable.thumbsupicon);
        textStatus = (TextView)view.findViewById(R.id.mainhome);
        textStatus.setText(R.string.recent_good);
    }

    private void startMqtt() {
        Log.d("Debug", "starting mqtt");
        mqttHelper = new MqttHelper(getActivity());
        Log.d("Debug", "started mqtt");

        mqttHelper.setCallback(new MqttCallbackExtended() {
            @Override
            public void connectComplete(boolean b, String s) {

            }

            @Override
            public void connectionLost(Throwable throwable) {

            }

            @Override
            public void messageArrived(String topic, MqttMessage mqttMessage) throws Exception {
                Log.d("Debug", "message gotten");
                Log.d("Debug", topic);
                Log.d("Debug", mqttMessage.toString());

                NotificationCompat.Builder builder = new NotificationCompat.Builder(getActivity());
                builder
                        .setContentTitle("Title")
                        .setContentText("content")
                        .setSmallIcon(R.mipmap.ic_launcher)
                        .setVisibility(NotificationCompat.VISIBILITY_PUBLIC);//to show content in lock screen

                textStatus.setText(mqttMessage.toString());
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {

            }
        });


    }
}