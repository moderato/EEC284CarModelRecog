LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#opencv
OPENCVROOT:= /Users/moderato/Downloads/OpenCV-android-sdk
OPENCV_CAMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=SHARED
include ${OPENCVROOT}/sdk/native/jni/OpenCV.mk

LOCAL_SRC_FILES := com_example_moderato_cvcv_OpencvNativeClass.cpp
LOCAL_LDLIBS += -llog
LOCAL_MODULE := OpencvNativeClass

include $(BUILD_SHARED_LIBRARY)