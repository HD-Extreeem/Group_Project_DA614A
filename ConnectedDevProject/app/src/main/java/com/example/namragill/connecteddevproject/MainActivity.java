package com.example.namragill.connecteddevproject;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    Button loginbtn;
    EditText name;
    EditText password;
    TextView hint;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        loginbtn = (Button)findViewById(R.id.btn);
        loginbtn.setOnClickListener(this);
        name = (EditText) findViewById(R.id.name);
        password = (EditText) findViewById(R.id.password);
        hint = (TextView) findViewById(R.id.hint);
        name.setHintTextColor(Color.WHITE);
        password.setHintTextColor(Color.WHITE);

        hint.setOnClickListener(this);

    }

    public void validate(String userName, String userPassword){
        if((userName.equals("admin")) && (userPassword.equals("admin"))){
            Intent intent = new Intent(this, ControllerActivity.class);
            startActivity(intent);
        }
        else{
            Toast.makeText(this, "Incorrect username or password", Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.hint:
                Toast.makeText(this, "Hint: you are the admin...", Toast.LENGTH_LONG).show();
                break;
            case R.id.btn:
                validate(name.getText().toString(), password.getText().toString());
                break;
        }

    }
}
