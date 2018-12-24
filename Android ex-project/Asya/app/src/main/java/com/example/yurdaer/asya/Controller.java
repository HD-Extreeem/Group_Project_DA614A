package com.example.yurdaer.asya;

/**
 * Created by YURDAER on 2018-01-20.
 */

import android.content.Intent;
import android.util.Log;
import android.widget.Toast;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.MqttException;

import java.io.UnsupportedEncodingException;


public class Controller {
    private MqttAndroidClient client;
    private PahoMqttClient pahoMqttClient;
    private MainActivity mainActivity;
    private Intent serviceIntent;
    private String powerStatus = "";

    public Controller(MainActivity mainActivity, boolean phoneRotation) {
        this.mainActivity = mainActivity;
        initMQTT(phoneRotation);
    }

    /* Connect to server and start service.
   * on phone rotation don't start a new service because the old one is already running */
    private void initMQTT(boolean phoneRotation) {
        pahoMqttClient = new PahoMqttClient();
        client = pahoMqttClient.getMqttClient(mainActivity.getApplicationContext(), Constants.MQTT_BROKER_URL, Constants.CLIENT_ID);
        serviceIntent = new Intent(mainActivity, MqttMessageService.class);
        if (!phoneRotation) {
            mainActivity.startService(serviceIntent);
        }


    }


    private void sendToServer(String msg) {
        try {
            pahoMqttClient.publishMessage(client, msg, 1, Constants.PUBLISH_TOPIC);
        } catch (MqttException e) {
            e.printStackTrace();
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }

    }

    public void onPause() {
    }

    public void onResume() {
    }

    public void onDestroy() {
        if (mainActivity.isFinishing()) {
            try {
                if (client != null && client.isConnected()) {
                    client.disconnect();
                }
            } catch (MqttException e) {
                e.printStackTrace();
            }
            mainActivity.stopService(serviceIntent);
        }
    }

    public void buttonClicked(boolean deviceStatus) {
        if (!deviceStatus)
            sendToServer("ON");
        else
            sendToServer("OFF");
    }


    public void setMqttStatus(boolean bol) {
        if (bol) {
            Toast toast = Toast.makeText(mainActivity.getApplicationContext(), "Connected", Toast.LENGTH_LONG);
            toast.show();
            subscribe();
        } else {
            Toast toast = Toast.makeText(mainActivity.getApplicationContext(), "Connection is lost", Toast.LENGTH_LONG);
            toast.show();
        }
    }

    public void onMessageArrived(String msg) {
        if (msg.contains("RUTIN")) {
            String[] arr = msg.split("-");
            mainActivity.setPowerStatus(powerStatus = arr[1]);
        } else {
            switch (msg) {
                case "DEVICE IS ON":
                    mainActivity.setDeviceStatus(true);
                    break;
                case "DEVICE IS OFF":
                    mainActivity.setDeviceStatus(false);
                    break;
                case "MOTION":
                    mainActivity.startAlarm();
            }

            Toast toast = Toast.makeText(mainActivity.getApplicationContext(), msg, Toast.LENGTH_LONG);
            toast.show();
        }
    }


    public void subscribe() {
        try {
            pahoMqttClient.subscribe(client, Constants.IN_TOPIC, 1);
            Log.i("*************", "Successfully subscribed**");
        } catch (MqttException e) {
            e.printStackTrace();
            Log.i("*************", "Failed to subscribed**");
        }
    }
}
