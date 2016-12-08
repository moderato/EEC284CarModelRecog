package com.example.moderato.cvcv;

import android.hardware.Camera;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceView;
import android.widget.SeekBar;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.JavaCameraView;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.MatOfRect;
import org.opencv.core.Point;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;
import org.opencv.objdetect.CascadeClassifier;

import rx.Observable;
import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Func1;
import rx.schedulers.Schedulers;

import static android.R.attr.y;

public class MainActivity extends AppCompatActivity implements CameraBridgeViewBase.CvCameraViewListener2 {

    private static final String TAG = "MainActivity";
    private JavaCameraView javaCameraView;
    private Mat mRgba, mGray;
    private VerticalSeekBar seekBar;
    private int highThreshold = 0;
    private String XMLDIR;
    private CascadeClassifier mClassifier;
    private MatOfRect mRects;
    private static final Scalar CAR_RECT_COLOR = new Scalar(0, 255, 0, 255);
    private int count = 0;

    BaseLoaderCallback mLoaderCallBack = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case BaseLoaderCallback.SUCCESS:
                    Camera camera = Camera.open();
                    Camera.Parameters params = camera.getParameters();
                    params.setPreviewFrameRate(15); // Set preview frame rate to 15;
                    camera.release();
                    javaCameraView.enableView();
                    break;
                default:
                    super.onManagerConnected(status);
                    break;
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        XMLDIR = CvHelper.copyClassifier(getApplicationContext());
        mClassifier = new CascadeClassifier(XMLDIR);
        if (mClassifier.empty()) {
            Log.e(TAG, "Loading classifier fail!!!");
        } else {
            Log.e(TAG, "Loading classifier succeed!!!");
        }

        javaCameraView = (JavaCameraView) findViewById(R.id.java_camera_view);
        javaCameraView.setVisibility(SurfaceView.VISIBLE);
        javaCameraView.setCvCameraViewListener(this);
        javaCameraView.setMaxFrameSize(400, 300); // 352*288

        seekBar = (VerticalSeekBar) findViewById(R.id.seekBar);
        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                highThreshold = progress;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
            }
        });
    }

    static {
        System.loadLibrary("opencv_java3");
        System.loadLibrary("OpencvNativeClass");
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (javaCameraView != null) {
            javaCameraView.disableView();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (javaCameraView != null) {
            javaCameraView.disableView();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (!OpenCVLoader.initDebug()) {
            Log.d(TAG, "OpenCV not loaded");
            mLoaderCallBack.onManagerConnected(LoaderCallbackInterface.SUCCESS);
        } else {
            Log.d(TAG, "OpenCV loaded！");
            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_1_0, this, mLoaderCallBack);
        }
    }

    @Override
    public void onCameraViewStarted(int width, int height) {
        mRgba = new Mat(height, width, CvType.CV_8UC4);
        mGray = new Mat(height, width, CvType.CV_8UC1);
        mRects = new MatOfRect();
    }

    @Override
    public void onCameraViewStopped() {
        mRgba.release();
        mGray.release();
        mRects.release();
    }

    @Override
    public Mat onCameraFrame(CameraBridgeViewBase.CvCameraViewFrame inputFrame) {
        mRgba = inputFrame.rgba();

        final Long addrObj = Long.valueOf(mRgba.getNativeObjAddr());

        if(count % 4 == 0) {
            Observable.just(addrObj)
                .flatMap(new Func1<Long, Observable<?>>() {
                    @Override
                    public Observable<Boolean> call(Long addr) {
                        long startTime = System.currentTimeMillis();
                        OpencvNativeClass.detectObject(addr, mRects.getNativeObjAddr(), XMLDIR);
                        Log.d(TAG, "Running time of the algorithm on this frame is " + (System.currentTimeMillis() - startTime));
                        return Observable.just(true);
                    }
                })
                .subscribeOn(Schedulers.computation())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe();
        }
        if(!mRects.empty()) {
            Rect[] carsArray = mRects.toArray();
            for (Rect car : carsArray) {
                Point pt1 = car.tl();
                pt1.y += 72; // Shift the rectangle for a distance of quarter of frame height
                Point pt2 = car.br();
                pt2.y += 72;
                Imgproc.rectangle(mRgba, pt1, pt2, CAR_RECT_COLOR, 3);
            }
        }
        count++;
        return mRgba;
    }
}
