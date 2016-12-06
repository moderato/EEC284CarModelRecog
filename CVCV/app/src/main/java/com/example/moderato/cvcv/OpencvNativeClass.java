package com.example.moderato.cvcv;

public class OpencvNativeClass {
    public static native int cannyThreshold(long matAddrSrc, long matAddrDst, int thresh1, int thresh2);
    public static native int detectObject(long matAddrSrc, long rectAddr, String xmlDir);
}

// cd app/src/main
// javah -d jni -classpath ../../build/intermediates/classes/debug com.example.moderato.cvcv.OpencvNativeClass
