#include <jni.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/core/core.hpp>
#include <stdio.h>
#include <string>
#include <cstdio>
#include <cstdlib>
/* Header for class com_example_moderato_cvcv_OpencvNativeClass */

using namespace std;
using namespace cv;

#ifndef _Included_com_example_moderato_cvcv_OpencvNativeClass
#define _Included_com_example_moderato_cvcv_OpencvNativeClass
#ifdef __cplusplus
extern "C" {
#endif

int cannyEdge(Mat img, Mat& detected);

/*
 * Class:     com_example_moderato_cvcv_OpencvNativeClass
 * Method:    cannyThreshold
 * Signature: (JJII)I
 */
JNIEXPORT jint JNICALL Java_com_example_moderato_cvcv_OpencvNativeClass_cannyThreshold
  (JNIEnv *, jclass, jlong, jlong, jint, jint);

/*
 * Class:     com_example_moderato_cvcv_OpencvNativeClass
 * Method:    detectObject
 * Signature: (JJII)I
 */
JNIEXPORT jint JNICALL Java_com_example_moderato_cvcv_OpencvNativeClass_detectObject
  (JNIEnv *, jclass, jlong, jstring);

#ifdef __cplusplus
}
#endif
#endif
