#include <com_example_moderato_cvcv_OpencvNativeClass.h>

int cannyEdge(Mat img, Mat& detected, int th1, int th2) {
    cvtColor(img, detected, CV_RGB2GRAY);
    blur(detected, detected, Size(3, 3));
    Canny(detected, detected, th1, th2, 3);
    if (detected.rows == img.rows && detected.cols == img.cols) {
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
  (JNIEnv *env, jclass, jlong addrSrc, jstring xmlDir){
    Mat& frame = *(Mat*)addrSrc;
    const char *dir = env->GetStringUTFChars(xmlDir, 0);
    CascadeClassifier face_cascade;
    if( !face_cascade.load(dir) ){ printf("--(!)Error loading\n"); return -1; };

    vector<Rect> faces;
    Mat frame_gray;

    cvtColor(frame, frame_gray, CV_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30), Size(70, 70));

    for(size_t i = 0; i < faces.size(); i++)
    {
        Point pt1(faces[i].x, faces[i].y);
        Point pt2(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
        rectangle(frame, pt1, pt2, Scalar(255, 0, 255), 4, 8, 0);
    }
    return 1;
  }