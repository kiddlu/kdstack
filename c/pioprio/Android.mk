LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := pioprio.c
LOCAL_MODULE := pioprio
include $(BUILD_EXECUTABLE)
