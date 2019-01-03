package com.example.namragill.connecteddevproject;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    Button loginbtn;
    EditText name;
    EditText password;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        loginbtn = (Button)findViewById(R.id.btn);
        loginbtn.setOnClickListener(this);

        name = (EditText) findViewById(R.id.name);
        password = (EditText) findViewById(R.id.password);
    }

    public void validate(String userName, String userPassword){
        if((userName.equals("admin")) && (userPassword.equals("admin"))){
            Intent intent = new Intent(this, ControllerActivity.class);
            startActivity(intent);
        }
    }

    @Override
    public void onClick(View v) {
        validate(name.getText().toString(), password.getText().toString());
    }
}
