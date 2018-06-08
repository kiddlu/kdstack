LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= verify.cpp
LOCAL_MODULE:= verify

include $(BUILD_EXECUTABLE)
