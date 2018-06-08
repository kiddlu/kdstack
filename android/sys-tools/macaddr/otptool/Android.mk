LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= otp_tool.c
LOCAL_MODULE:= otptool

LOCAL_SHARED_LIBRARIES := libc libcutils

include $(BUILD_EXECUTABLE)
