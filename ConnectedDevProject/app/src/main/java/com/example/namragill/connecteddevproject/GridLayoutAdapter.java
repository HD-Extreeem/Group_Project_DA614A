package com.example.namragill.connecteddevproject;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;

import java.util.ArrayList;

/**
 * Created by hadideknache on 2018-12-01.
 */

public class GridLayoutAdapter extends RecyclerViewAdapter implements View.OnClickListener {
    private Activity act;
    private ArrayList<Bitmap> img;
    private ArrayList<Bitmap> imgToShow;
    public static int pos;
    ViewImage viewImage;
    DashFragment dashFragment;

    public GridLayoutAdapter(Activity act, ArrayList<Bitmap> img) {
        this.act = act;
        this.img = img;
        viewImage  = new ViewImage();
        dashFragment = new DashFragment();
    }

    @Override
    public CustomRecycleViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(act).inflate(R.layout.images, parent, false);
        Holder holder = new Holder(view);
        view.setOnClickListener(this);
        return holder;
    }

    @Override
    public void onBindViewHolder(final CustomRecycleViewHolder holder, final int position) {
        final Holder myHolder = (Holder) holder;
        myHolder.images.setImageBitmap(img.get(position));
        this.pos = position;
     //   viewImage.setImage(img.get(position));
    }

    @Override
    public int getItemCount() {

        return img.size();
    }

    @Override
    public void onClick(View v) {
        Context context = v.getContext();
        imgToShow = dashFragment.getImage();
        Log.d("LayoutAdapte", String.valueOf(imgToShow));
        viewImage.setImage(imgToShow,1);
        Log.d("onClick","show");
        Intent intent = new Intent(context, ViewImage.class);

        context.startActivity(intent);
    }

    public class Holder extends CustomRecycleViewHolder {
        private ImageView images;

        public Holder(View itemView) {
            super(itemView);
            images = (ImageView) itemView.findViewById(R.id.images);
        }
    }
}
