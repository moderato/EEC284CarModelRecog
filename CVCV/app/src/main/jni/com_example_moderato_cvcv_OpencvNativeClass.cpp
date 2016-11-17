#include <com_example_moderato_cvcv_OpencvNativeClass.h>

int cannyEdge(Mat img, Mat& detected, int th1, int th2) {
  cvtColor(img, detected, CV_RGB2GRAY);
  blur(detected, detected, Size(3,3));
  Canny(detected, detected, th1, th2, 3);
  if (detected.rows == img.rows && detected.cols==img.cols) {
    return 1;
  }
  return 0;
}

JNIEXPORT jint JNICALL Java_com_example_moderato_cvcv_OpencvNativeClass_cannyThreshold
  (JNIEnv *, jclass, jlong addrSrc, jlong addrDst, jint thresh1, jint thresh2){
    Mat& src = *(Mat*)addrSrc;
    Mat& dst = *(Mat*)addrDst;
    int th1 = (int) thresh1;
    int th2 = (int) thresh2;

    int conv;
    jint retVal;
    conv = cannyEdge(src, dst, th1, th2);

    retVal = (jint) conv;
    return retVal;
  };

JNIEXPORT jint JNICALL Java_com_example_moderato_cvcv_OpencvNativeClass_detectObject
  (JNIEnv *, jclass, jlong addrSrc, jstring xmlDir){
    Mat& src = *(Mat*)addrSrc;


    return 1;
  }