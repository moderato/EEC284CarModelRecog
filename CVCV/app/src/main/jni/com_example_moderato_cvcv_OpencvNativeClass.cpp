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
  (JNIEnv *env, jclass, jlong addrSrc, jstring xmlDir){
    Mat& src = *(Mat*)addrSrc;
    const char *dir = env->GetStringUTFChars(xmlDir, 0);
    CvHaarClassifierCascade *pHaarCascade = NULL;
    pHaarCascade = (CvHaarClassifierCascade*)cvLoad(dir);

    IplImage* pSrcImage;
    *pSrcImage = IplImage(src);
    IplImage *pGrayImage = cvCreateImage(cvGetSize(pSrcImage), IPL_DEPTH_8U, 1);
    cvCvtColor(pSrcImage, pGrayImage, CV_BGR2GRAY);

    if (pHaarCascade != NULL)
      {
        CvScalar FaceCirclecolors[] =
        {
            CvScalar(0, 0, 255),
            CvScalar(0, 128, 255),
            CvScalar(0, 255, 255),
            CvScalar(0, 255, 0),
            CvScalar(255, 128, 0),
            CvScalar(255, 255, 0),
            CvScalar(255, 0, 0),
            CvScalar(255, 0, 255)
        };

        CvMemStorage *pcvMStorage = cvCreateMemStorage(0);
        cvClearMemStorage(pcvMStorage);

        CvSeq *pcvSeqFaces = cvHaarDetectObjects(pGrayImage, pHaarCascade, pcvMStorage);

        for(int i = 0; i <pcvSeqFaces->total; i++)
        {
            CvRect* r = (CvRect*)cvGetSeqElem(pcvSeqFaces, i);
            CvPoint center;
            int radius;
            center.x = cvRound((r->x + r->width * 0.5));
            center.y = cvRound((r->y + r->height * 0.5));
            radius = cvRound((r->width + r->height) * 0.25);
            cvCircle(pSrcImage, center, radius, FaceCirclecolors[i % 8], 2);
        }
        cvReleaseMemStorage(&pcvMStorage);
    }

    src = cvarrToMat(pSrcImage);
    cvReleaseImage(&pSrcImage);
    cvReleaseImage(&pGrayImage);
    return 1;
  }