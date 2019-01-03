package com.example.namragill.connecteddevproject;

import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.Vibrator;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.app.NotificationCompat;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

import static android.content.Context.NOTIFICATION_SERVICE;
import static android.content.Context.VIBRATOR_SERVICE;

/**
 * Created by namragill on 2018-11-14.
 */

public class HomeFragment extends Fragment {

    ImageView imageView;
    public static TextView textStatus;
    MqttHelper mqttHelper;
    Vibrator vibrator;

    final String serverUri = "tcp://m23.cloudmqtt.com:10941";


    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {

        vibrator = (Vibrator) getActivity().getSystemService(VIBRATOR_SERVICE);

        View view = inflater.inflate(R.layout.fragment_home, container, false);
        initiatecomponents(view);
        startMqtt();
        Log.d("Debug", "after method");
        return view;
    }

    private void initiatecomponents(View view) {
        imageView = (ImageView) view.findViewById(R.id.mainimg);
        imageView.setImageResource(R.drawable.thumbsupicon);
        textStatus = (TextView) view.findViewById(R.id.mainhome);
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
              //  textStatus.setText(mqttMessage.toString());
                textStatus.setText(new String(mqttMessage.getPayload()));
                vibrator.vibrate(500);
                addNotification();
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {

            }
        });


    }
    private void addNotification() {
        NotificationCompat.Builder builder =
                new NotificationCompat.Builder(getContext())
                        .setSmallIcon(R.drawable.ic_notifications_black_24dp)
                        .setContentTitle("Movement detection")
                        .setContentText("Unusual movement detected");

        Intent notificationIntent = new Intent(getActivity(),HomeFragment.class);
        PendingIntent contentIntent = PendingIntent.getActivity(getContext(), 0, notificationIntent,
                PendingIntent.FLAG_UPDATE_CURRENT);
        builder.setContentIntent(contentIntent);

        // Add as notification
        NotificationManager manager = (NotificationManager) getActivity().getSystemService(Context.NOTIFICATION_SERVICE);
        manager.notify(0, builder.build());
    }
}