package com.example.glbasicframework;

import android.app.Activity;
import android.os.Bundle;

public class GLBasicFrameworkActivity extends Activity
{
    CustomGLSurfaceView mView;

    @Override protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mView = new CustomGLSurfaceView(getApplication());
        setContentView(mView);
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }
}
