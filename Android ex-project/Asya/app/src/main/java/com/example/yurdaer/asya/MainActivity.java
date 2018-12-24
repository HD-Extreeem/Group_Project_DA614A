package com.example.yurdaer.asya;

import android.content.DialogInterface;
import android.media.Ringtone;
import android.media.RingtoneManager;
import android.net.Uri;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;

import static android.app.AlertDialog.THEME_DEVICE_DEFAULT_DARK;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    static Controller controller;
    private ImageButton imageButton;
    private TextView textView;
    private boolean deviceStatus = false;
    private Ringtone ringtone;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initComponents();
        initController(savedInstanceState);
    }

    private void initComponents() {
        imageButton = (ImageButton) findViewById(R.id.imageButton);
        textView = (TextView) findViewById(R.id.textView);
        imageButton.setOnClickListener(this);
    }

    private void initController(Bundle savedInstanceState) {
        boolean phoneRotation = false;
        if (savedInstanceState != null) {
            phoneRotation = true;
        }
        controller = new Controller(this, phoneRotation);

    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        if (deviceStatus) {
            outState.putBoolean("status", deviceStatus);
        }
    }

    @Override
    protected void onRestoreInstanceState(Bundle savedInstanceState) {
        super.onRestoreInstanceState(savedInstanceState);
        if (savedInstanceState != null) {
            setDeviceStatus(savedInstanceState.getBoolean("status"));
        }
    }

    public void setDeviceStatus(boolean deviceStatus) {
        if (deviceStatus) {
            imageButton.setImageResource(R.drawable.on);
            this.deviceStatus = true;
        } else {
            imageButton.setImageResource(R.drawable.off);
            this.deviceStatus = false;
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        controller.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        controller.onPause();

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        controller.onDestroy();

    }

    public void startAlarm() {
        Uri alert = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_ALARM);
        if (alert == null) {
            // alert is null, using backup
            alert = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);
            if (alert == null) {  // I can't see this ever being null (as always have a default notification) but just incase
                // alert backup is null, using 2nd backup
                alert = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_RINGTONE);
            }
        }
         ringtone = RingtoneManager.getRingtone(getApplicationContext(), alert);
        if (ringtone != null) {
            ringtone.play();
            AlertDialog.Builder builder1 = new AlertDialog.Builder(this,THEME_DEVICE_DEFAULT_DARK);
            builder1.setMessage("KALK KALK KALK");
            builder1.setCancelable(true);

            builder1.setPositiveButton(
                    "TAMAM",
                    new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                            ringtone.stop();
                            dialog.cancel();
                        }
                    });
            AlertDialog alert11 = builder1.create();
            alert11.show();
        }
    }

    public void setPowerStatus(String value) {
        int number = Integer.parseInt(value);
        if (number < 30 && number > 20) {
            textView.setTextColor(getResources().getColor(R.color.colorYELLOW));
        } else if (number <= 20) {
            textView.setTextColor(getResources().getColor(R.color.colorRED));
        } else {
            textView.setTextColor(getResources().getColor(R.color.colorGreen));
        }
        textView.setText(value+"%");
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.imageButton:
                controller.buttonClicked(deviceStatus);
                break;
        }
    }
}
