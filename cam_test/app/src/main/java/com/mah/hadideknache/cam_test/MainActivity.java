package com.mah.hadideknache.cam_test;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.support.design.widget.FloatingActionButton;

/**
 * Created by hadideknache on 2018-12-01.
 */

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initGui();
    }

    private void initGui() {
        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(getMain(), RecyclerViewDisplay.class);
                intent.putExtra("LayoutManager", "GridLayoutManager");
                startActivity(intent);
            }
        });
    }

    private MainActivity getMain(){
        return this;
    }

}
