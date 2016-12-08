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

void vector_Rect_to_Mat(vector<Rect>& v_rect, Mat& mat)
{
    mat = Mat(v_rect, true);
}

JNIEXPORT jint JNICALL Java_com_example_moderato_cvcv_OpencvNativeClass_cannyThreshold
  (JNIEnv *, jclass, jlong addrSrc, jlong addrDst, jint thresh1, jint thresh2) {
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
  (JNIEnv *env, jclass, jlong addrSrc, jlong rectAddr, jstring xmlDir) {

    Mat& frame = *((Mat*)addrSrc);
    Mat& cars = *((Mat*)rectAddr);
    vector<Rect> objs;
    Rect myROI(0, 72, 352, 144);
    Mat cropped = frame(myROI).clone();

    const char *dir = env->GetStringUTFChars(xmlDir, 0);
    CascadeClassifier face_cascade;
    if( !face_cascade.load(dir) ){ cout << "--(!)Error loading" << endl; return -1; };

    Mat frame_gray;

    cvtColor(cropped, frame_gray, CV_BGR2GRAY);
//    cvtColor(frame, frame_gray, CV_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    face_cascade.detectMultiScale(frame_gray, objs, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(20, 20), Size(80, 80));

//    for(size_t i = 0; i < objs.size(); i++) {
//        objs[i].y += 72;
//        Point pt1(objs[i].x, objs[i].y);
//        Point pt2(objs[i].x + objs[i].width, objs[i].y + objs[i].height);
//        rectangle(frame, pt1, pt2, Scalar(0, 255, 0), 4, 8, 0);
//    }

    vector_Rect_to_Mat(objs, cars);

    frame_gray.release();
    return 1;
  }