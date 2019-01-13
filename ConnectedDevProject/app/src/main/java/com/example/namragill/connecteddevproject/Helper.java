package com.example.namragill.connecteddevproject;

import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;


/**
 * Created by namragill on 2017-09-17.
 * This Class is the Database Helper
 */

public class Helper extends SQLiteOpenHelper {

    /*Initialising column and table names*/
    public static final String TABLE_NAME_MES = "log";
    public static final String COLUMN_VALUE = "message";
    public static final String COLUMN_DATE = "date";


    private static final String DATABASE_NAME = "data.db";
    private static final int DATABASE_VERSION = 1;


    // Database creation sql statement
    private static final String DATABASE_CREATE =
            "create table " + TABLE_NAME_MES + "(" + COLUMN_VALUE + " text not null primary key);";


    public Helper(Context context) {
        super(context, DATABASE_NAME, null, DATABASE_VERSION);
    }

    // Creating Tables
    @Override
    public void onCreate(SQLiteDatabase db) {
        db.execSQL(DATABASE_CREATE);
    }

    // Upgrading database
    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        Log.w(Helper.class.getName(),
                "Upgrading database from version " + oldVersion + " to "
                        + newVersion + ", which will destroy all old data");
        db.execSQL("DROP TABLE IF EXISTS " + TABLE_NAME_MES);
        onCreate(db);
    }

    /*This method gets all the data For Incomes*/
    public Cursor getAllData(){
        SQLiteDatabase db = this.getWritableDatabase();
        Cursor cursor = db.rawQuery("SELECT * FROM " + Helper.TABLE_NAME_MES ,null);
        return cursor;

    }

}
