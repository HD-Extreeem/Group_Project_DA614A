package com.example.namragill.connecteddevproject;

import android.app.Activity;
import android.graphics.Bitmap;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;

import java.util.ArrayList;

/**
 * Created by hadideknache on 2018-12-01.
 */

public class GridLayoutAdapter extends RecyclerViewAdapter {
    private Activity act;
    private ArrayList<Bitmap> img;

    public GridLayoutAdapter(Activity act, ArrayList<Bitmap> img) {
        this.act = act;
        this.img = img;
    }

    @Override
    public CustomRecycleViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(act).inflate(R.layout.images, parent, false);
        Holder holder = new Holder(view);
        return holder;
    }

    @Override
    public void onBindViewHolder(final CustomRecycleViewHolder holder, final int position) {
        final Holder myHolder = (Holder) holder;
        myHolder.images.setImageBitmap(img.get(position));
    }

    @Override
    public int getItemCount() {

        return img.size();
    }

    public class Holder extends CustomRecycleViewHolder {
        private ImageView images;

        public Holder(View itemView) {
            super(itemView);
            images = (ImageView) itemView.findViewById(R.id.images);
        }
    }
}
