package com.example.namragill.connecteddevproject;

import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.design.widget.BottomNavigationView;
import android.support.v4.app.Fragment;
import android.support.v7.app.AppCompatActivity;
import android.view.MenuItem;

public class ControllerActivity extends AppCompatActivity {



    private BottomNavigationView.OnNavigationItemSelectedListener mOnNavigationItemSelectedListener
            = new BottomNavigationView.OnNavigationItemSelectedListener() {

        @Override
        public boolean onNavigationItemSelected(@NonNull MenuItem item) {
            Fragment selectedFragment = null;
            switch (item.getItemId()) {
                case R.id.navigation_home:
                    selectedFragment = new HomeFragment();
                    getSupportFragmentManager().beginTransaction().replace(R.id.fHome,selectedFragment).commit();
                    return true;
                case R.id.navigation_dashboard:
                    selectedFragment = new DashFragment();
                    getSupportFragmentManager().beginTransaction().replace(R.id.fHome,selectedFragment).commit();
                    return true;
                case R.id.navigation_notifications:
                    selectedFragment = new NotifyFragment();
                    getSupportFragmentManager().beginTransaction().replace(R.id.fHome,selectedFragment).commit();
                    return true;
            }
            return true;
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_controller);
        getSupportFragmentManager().beginTransaction().replace(R.id.fHome,new HomeFragment());
        BottomNavigationView navigation = (BottomNavigationView) findViewById(R.id.navigation_view);
        navigation.setOnNavigationItemSelectedListener(mOnNavigationItemSelectedListener);

    }

}
