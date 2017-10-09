package com.example.glbasicframework;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;

class CustomGLSurfaceView extends GLSurfaceView {

    public CustomGLSurfaceView(Context context) {
        super(context);
        setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        setEGLContextClientVersion(3);
        setRenderer(new Renderer());
    }

    private static class Renderer implements GLSurfaceView.Renderer {
    	boolean created = false;
        public void onDrawFrame(GL10 gl) {
            Native.update();
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {
            Native.resize(width, height);
        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        	if(!created)
        	{
        		created = true;
                Native.init();
        	}
        }
    }
}
