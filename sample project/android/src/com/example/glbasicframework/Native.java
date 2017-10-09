package com.example.glbasicframework;

public class Native {

    static {
    	System.loadLibrary("3DBasicFramework");
    }

	public static native void init();
	public static native void resize(int width, int height);
	public static native void update();

}