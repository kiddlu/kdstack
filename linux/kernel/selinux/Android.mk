LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= setenforce.c
LOCAL_MODULE:= setenforce

include $(BUILD_EXECUTABLE)
