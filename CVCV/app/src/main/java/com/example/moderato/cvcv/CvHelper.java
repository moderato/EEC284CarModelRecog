package com.example.moderato.cvcv;

import android.content.Context;
import android.content.res.AssetManager;
import android.os.Environment;
import android.util.Log;

import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class CvHelper {
    private static final String TAG = "CvHelper";
    private static final String mStorageDir = Environment.getExternalStoragePublicDirectory(
            Environment.DIRECTORY_PICTURES).getAbsolutePath();

    public static String copyClassifier(Context context) {
        String mDirPath = mStorageDir;
        String mPath = mDirPath +  File.separator + "support" + File.separator + "car_detection.xml";

        // only copy if file is not already present
        if (!(new File(mPath).exists())) {
            try {
                AssetManager assetManager = context.getAssets();
                InputStream in = assetManager.open("support/car_detection.xml");

                File dir = new File(mDirPath);
                if (!dir.exists()) {
                    dir.mkdirs();
                    Log.d(TAG, "Make new directory");
                }

                File dataDir = new File(mDirPath, "support");
                if (!dataDir.exists()) {
                    dataDir.mkdirs();
                    Log.d(TAG, "Make new directory support");
                }

                File dataFile = new File(dataDir, "car_detection.xml");
                DataOutputStream out = new DataOutputStream(new FileOutputStream(
                        dataFile.getAbsolutePath()));

                byte[] buf = new byte[80000];
                int len;
                while ((len = in.read(buf)) > 0) {
                    out.write(buf, 0, len);
                }

                in.close();
                out.close();
            } catch (IOException e) {
                android.util.Log.e("Error", e.toString());
            }
        }
        return mPath;
    }
}
