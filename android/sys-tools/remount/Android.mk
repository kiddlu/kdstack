LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= remount.cpp

LOCAL_STATIC_LIBRARIES := \
    libfs_mgr \
    libcutils \
    libbase

LOCAL_MODULE:= remount

include $(BUILD_EXECUTABLE)
