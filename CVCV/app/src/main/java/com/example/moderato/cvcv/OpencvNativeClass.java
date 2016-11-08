package com.example.moderato.cvcv;

/**
 * Created by moderato on 2016/11/7.
 */
public class OpencvNativeClass {
    public static native int convertGray(long matAddrRgba, long matAddrGray);
}

// cd app/src/main
// javah -d jni -classpath ../../build/intermediates/classes/debug com.example.moderato.cvcv.OpencvNativeClass
